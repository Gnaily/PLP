#include "./CskMachine.hpp"

#include <memory>
#include <string>
#include <map>
#include <list>
#include "../ast/AstNode.hpp"
#include "../ast/Program.hpp"
#include "../ast/Stmt.hpp"
#include "../ast/Block.hpp"
#include "../ast/Expr.hpp"
#include "../ast/Error.hpp"
namespace csk
{
  using namespace std;
  using namespace ast;
  using Ctrl = variant<shared_ptr<Expr>, string>;

  class State
  {
  private:
    Ctrl ctrl;
    map<string, float> sto;
    Program &kont;

  public:
    State() = default;
    State(string str, map<string, float> sto, Program &kont)
        : ctrl(str), sto(sto), kont(kont) {
          };

    variant<shared_ptr<Expr>, string> &control()
    {
      return this->ctrl;
    };

    map<string, float> &store()
    {
      return this->sto;
    };

    Program &kontination()
    {
      return this->kont;
    };

    ~State() = default;
  };

  // ---------------------declare helper function-----------------------------
  State load(Program &program);
  State &transition(State &state);
  variant<float, string> unload(State &state);
  float value(Ctrl &ctrl, map<string, float> &store);
  void push(shared_ptr<Block> block, list<Tstmt> &kont);
  bool is_final(State &state);
  // ---------------------helper end--------------------------

  variant<float, string> CskMachine::run(Program &program)
  {

    State state = load(program);
    while (!is_final(state))
    {
      transition(state);
    }
    return unload(state);
  }

  const string SEARCH{"i am searching next instruction..."};
  const string ANSER{"_ANSER_"};
  State load(Program &program)
  {
    const string &search = SEARCH;
    map<string, float> map{};
    State initail{search, map, program};
    return initail;
  }

  State &transition(State &state)
  {
    Ctrl &ctrl = state.control();
    Program &kont = state.kontination();
    map<string, float> &store = state.store();
    list<Tstmt> &stmts = kont.getStmts();
    if (holds_alternative<string>(ctrl))
    {
      // ctrl = ...
      // store don't change
      // kont don't change
      if (!stmts.empty())
      {
        Tstmt &first = stmts.front();
        Statment *stmt = get<shared_ptr<Statment>>(first).get();
        if (Assign *ass = static_cast<Assign *>(stmt))
        {
          ctrl = ass->getExpr();
        }
        else if (If *if0 = static_cast<If *>(stmt))
        {
          ctrl = if0->getTest();
        }
        else if (While *while0 = static_cast<While *>(stmt))
        {
          ctrl = while0->getTest();
        }
      }
      else
      {
        ctrl = get<shared_ptr<Expr>>(kont.getExpr());
      }
    }
    else
    {
      // value =  value(ctrl)
      // store = ...
      // kont = ...
      float v = value(ctrl, store);
      list<Tstmt> &stmts = kont.getStmts();
      if (!stmts.empty())
      {
        Tstmt &first = stmts.front();
        Statment *stmt = get<shared_ptr<Statment>>(first).get();
        if (Assign *ass = static_cast<Assign *>(stmt))
        {
          ctrl = SEARCH;
          // store = ...
          Variable &var = ass->getVar();
          const string varName = var.getName();
          store[varName] = v;

          // kont = ...
          stmts.pop_front();
          // assign disappered , kont = rest program
        }
        else if (If *if0 = static_cast<If *>(stmt))
        {
          if (v == 1)
          {
            ctrl = SEARCH;

            // stmts_of_then = ...then...
            // kont =  add(stmt_of_then, kont)
            shared_ptr<Block> then = if0->getThen();
            push(then, kont.getStmts());
          }
          else
          {
            ctrl = SEARCH;
            // stmts_of_else = ...els...
            // kont =  add(stmt_of_else, kont)
            shared_ptr<Block> els = if0->getElse();
            push(els, kont.getStmts());
          }
        }
        else if (While *while0 = static_cast<While *>(stmt))
        {
          ctrl = SEARCH;
          // stmt_of_body = ...body...
          // kont =  add(stmt_of_body, kont)
          shared_ptr<Block> body = while0->getBody();
          push(body, kont.getStmts());
        }
      }
      else
      {
        Texpr& expr = kont.getExpr();
        if (get<shared_ptr<Expr>>(expr) == nullptr)
        {
          store[ANSER] = v;
          expr = nullptr; 
        }
        else 
        {
          ctrl = SEARCH;
        }
      }
    }
    return state;
  }

  void push(shared_ptr<Block> block, list<Tstmt> &kont)
  {
    if (LabledBlock *lb = static_cast<LabledBlock *>(block.get()))
    {
      list<Tstmt> &stmts = lb->getStatments();
      for (auto &item : stmts)
      {
        kont.push_front(get<shared_ptr<Statment>>(item));
      }
    }
    else if (StmtBlock *sb = static_cast<StmtBlock *>(block.get()))
    {
      shared_ptr<Statment> stmt = sb->getStatment();
      kont.push_front(stmt);
    }
    else
    {
      abort();
    }
  }

  // evaluate current expr's value in the store
  float value(Ctrl &ctrl, map<string, float> &store)
  {
    shared_ptr<Expr> expr = get<shared_ptr<Expr>>(ctrl);

    if (Number *num = static_cast<Number *>(expr.get()))
    {
      return num->getNumber();
    }
    else if (Variable *var = static_cast<Variable *>(expr.get()))
    {
      const string name = var->getName();
      return store[name];
    }
    else if (BinOp *binOp = static_cast<BinOp *>(expr.get()))
    {
      const string op = binOp->op();
      if (op.compare("+") == 0)
      {
        const string rightVar = binOp->right().getName();
        const string leftVar = binOp->left().getName();
        float rightVal = store[rightVar];
        return store[leftVar] + rightVal;
      }
      else if (op.compare("/") == 0)
      {
        const string rightVar = binOp->right().getName();
        const string leftVar = binOp->left().getName();
        float rightVal = store[rightVar];
        return store[leftVar] / rightVal;
      }
      else if (op.compare("==") == 0)
      {
        const string rightVar = binOp->right().getName();
        const string leftVar = binOp->left().getName();
        float rightVal = store[rightVar];
        // TODO
        return store[leftVar] == rightVal;
      }
      else
      {
        abort();
      }
    }
    else
    {
      abort();
    }
  }

  bool is_final(State &state)
  {
    Program &kont = state.kontination();
    list<Tstmt> &stmts = kont.getStmts();
    shared_ptr<Expr> expr = get<shared_ptr<Expr>>(kont.getExpr());
    return expr.get() == nullptr;
  }

  variant<float, string> unload(State &state)
  {
    map<string, float> &store = state.store();
    return store[ANSER];
  }
}