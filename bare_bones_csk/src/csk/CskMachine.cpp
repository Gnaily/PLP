#include "./CskMachine.hpp"
#include "../ast/AstNode.hpp"
#include "../ast/Program.hpp"
#include "../ast/Stmt.hpp"
#include "../ast/Block.hpp"
#include "../ast/Expr.hpp"
#include "../ast/Error.hpp"
#include <iostream>
namespace csk
{
  using namespace std;
  using namespace ast;

  bool State::isInitial()
  {
    return false;
  }

  bool State::isFinal()
  {
    return false;
  }

  Initial::Initial(Program &kont) : kont(kont) {}

  bool Initial::isInitial()
  {
    return true;
  }

  STORE(Initial);
  CONTINATION(Initial);

  Intermidate::Intermidate(Ctrl str, map<string, float> stor, Program &kont)
      : ctrl(str), stor(stor), kont(kont) {}

  variant<shared_ptr<Expr>, string> &Intermidate::control()
  {
    return this->ctrl;
  }

  STORE(Intermidate);
  CONTINATION(Intermidate);

  Final::Final(Result ctrl, map<string, float> stor, Program &kont)
      : ctrl(ctrl), stor(stor), kont(kont) {}

  bool Final::isFinal()
  {
    return true;
  }

  Result Final::getResult()
  {
    return this->ctrl;
  }

  // ---------------------declare helper function for CskMachine--------------------
  Initial load(Program &program);
  State &transition(State &state);
  variant<float, string> unload(State &state);
  variant<float, string> value(Ctrl &ctrl, map<string, float> &store);
  void push(shared_ptr<Block> block, list<Tstmt> &kont);

  // -------------------------------------------------------------------------------
  CskMachine::CskMachine() {}

  variant<float, string> CskMachine::run(Program &program)
  {
    Initial initial = load(program);
    reference_wrapper<State> stateRef = initial;
    while (!stateRef.get().isFinal())
    {
      stateRef = transition(stateRef);
    }
    return unload(stateRef);
  }

  Initial load(Program &program)
  {
    return Initial{program};
  }

  State &transition(State &state)
  {
    if (state.isInitial())
    {
      Initial *state_ptr = dynamic_cast<Initial *>(&state);
      static Intermidate intmState{SEARCH, state_ptr->store(), state_ptr->contination()};
      State &ref = intmState;
      return ref;
    }

    Intermidate *state_ptr = dynamic_cast<Intermidate *>(&state);
    if (!state_ptr)
    {
      abort();
    }

    Ctrl &ctrl = state_ptr->control();
    map<string, float> &store = state_ptr->store();
    Program &kont = state_ptr->contination();
    list<Tstmt> &stmts = kont.getStmts();
    if (holds_alternative<string>(ctrl))
    {
      // ctrl = ...
      // store don't change
      // kont don't change
      if (stmts.empty())
      {
        ctrl = get<shared_ptr<Expr>>(kont.getExpr());
        return state;
      }

      Tstmt &first = stmts.front();
      Statment *stmt = get<shared_ptr<Statment>>(first).get();

      if (Assign *ass = dynamic_cast<Assign *>(stmt))
      {
        ctrl = ass->getExpr();
      }
      else if (If *if0 = dynamic_cast<If *>(stmt))
      {
        ctrl = if0->getTest();
      }
      else if (While *while0 = dynamic_cast<While *>(stmt))
      {
        ctrl = while0->getTest();
      }
      else
      {
        cout << "@Stuck state: no statment match" << endl;
        abort();
      }
      return state;
    }
    else
    {
      // value =  value(ctrl, store)
      // store = ...
      // kont = ...
      variant<float, string> val = value(ctrl, store);
      if (holds_alternative<string>(val))
      {
        static Final final{get<string>(val), store, kont};
        return final;
      }
      float v = get<float>(val);
      list<Tstmt> &stmts = kont.getStmts();
      if (!stmts.empty())
      {
        Tstmt &first = stmts.front();
        Statment *stmt = get<shared_ptr<Statment>>(first).get();
        if (Assign *ass = dynamic_cast<Assign *>(stmt))
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
        else if (If *if0 = dynamic_cast<If *>(stmt))
        {
          if (v == 1)
          {
            ctrl = SEARCH;

            // stmts_of_then = ...then...
            // kont =  add(stmt_of_then, kont)
            shared_ptr<Block> then = if0->getThen();
            stmts.pop_front();
            push(then, stmts);
          }
          else
          {
            ctrl = SEARCH;
            // stmts_of_else = ...els...
            // kont =  add(stmt_of_else, kont)
            shared_ptr<Block> els = if0->getElse();
            stmts.pop_front();
            push(els, stmts);
          }
        }
        else if (While *while0 = dynamic_cast<While *>(stmt))
        {
          ctrl = SEARCH;
          // stmt_of_body = ...body...
          // kont =  add(stmt_of_body, kont)
          if (v == 1)
          {
            ctrl = SEARCH;
            // stmts_of_else = ...els...
            // kont =  add(stmt_of_else, kont)
            shared_ptr<Block> body = while0->getBody();
            push(body, kont.getStmts());
          }
          else
          {
            ctrl = SEARCH;
            // stmts_of_else = ...els...
            // kont =  add(stmt_of_else, kont)
            stmts.pop_front();
          }
        }
        else
        {
          cout << "@Stuck: no statment match" << endl;
          abort();
        }
      }
      else
      {
        static Final final{v, store, kont};
        return final;
      }
      return state;
    }
  }

  void push(shared_ptr<Block> block, list<Tstmt> &kont)
  {
    if (LabledBlock *lb = dynamic_cast<LabledBlock *>(block.get()))
    {
      list<Tstmt> &stmts = lb->getStatments();
      for (auto &item : stmts)
      {
        kont.push_front(get<shared_ptr<Statment>>(item));
      }
    }
    else if (StmtBlock *sb = dynamic_cast<StmtBlock *>(block.get()))
    {
      shared_ptr<Statment> stmt = sb->getStatment();
      kont.push_front(stmt);
    }
    else
    {
      cout << "@Stuck: no block match" << endl;
      abort();
    }
  }

  // evaluate current expr's value in the store
  variant<float, string> value(Ctrl &ctrl, map<string, float> &store)
  {
    shared_ptr<Expr> expr = get<shared_ptr<Expr>>(ctrl);
    if (Number *num = dynamic_cast<Number *>(expr.get()))
    {
      return num->getNumber();
    }
    else if (Variable *var = dynamic_cast<Variable *>(expr.get()))
    {
      const string &name = var->getName();
      if (store.count(name) > 0)
      {
        return store[name];
      }
      string report{"error:"};
      return report.append("variable of ").append(name).append(" is not defined");
    }
    else if (BinOp *binOp = dynamic_cast<BinOp *>(expr.get()))
    {

      const string leftVar = binOp->left().getName();
      const string rightVar = binOp->right().getName();
      if (!(store.count(leftVar) > 0 && store.count(rightVar) > 0))
      {
        string report{"error:"};
        return report.append("variable of ")
            .append(leftVar)
            .append(" or ")
            .append(rightVar)
            .append(" is not defined");
      }

      const string op = binOp->op();
      if (op.compare("+") == 0)
      {
        float rightVal = store[rightVar];
        return store[leftVar] + rightVal;
      }
      else if (op.compare("/") == 0)
      {

        if (store[rightVar] == 0)
        {
          string report{"error:"};
          return report
              .append("in divide expression, the ")
              .append(rightVar)
              .append(" can't be zero ");
        }

        float rightVal = store[rightVar];
        return store[leftVar] / rightVal;
      }
      else if (op.compare("==") == 0)
      {
        float rightVal = store[rightVar];
        return store[leftVar] == rightVal;
      }
      else
      {
        cout << "@Stuck: no operator match" << endl;
        abort();
      }
    }
    else
    {
      cout << "@Stuck: no Expr match" << endl;
      abort();
    }
  }

  variant<float, string> unload(State &state)
  {
    Final *state_ptr = dynamic_cast<Final *>(&state);
    if (!state_ptr)
    {
      cout << "@Stuck:not final state" << endl;
      abort();
    }
    return state_ptr->getResult();
  }
}