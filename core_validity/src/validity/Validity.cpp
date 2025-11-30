#include <iostream>
#include <memory>
#include <variant>
#include <set>
#include "Validity.hpp"

namespace validity
{
  using namespace ast;

  bool closed(Variable &var, set<string> &declared);
  Texpr closed(const shared_ptr<Expr> &expr, set<string> &declared);

  list<Tdecl> closed(list<Tdecl> &decls, set<string> &declared);
  Tdecl closed(Tdecl &decl, set<string> &declared);

  list<Tstmt> closed(list<Tstmt> &stmts, set<string> declared);
  shared_ptr<Statment> closed(shared_ptr<Statment> &stmt, set<string> declared);

  shared_ptr<Assign> closed(const shared_ptr<Assign> &ass, set<string> declared);
  shared_ptr<If> closed(const shared_ptr<If> &if0, set<string> declared);
  shared_ptr<While> closed(const shared_ptr<While> &while0, set<string> declared);

  shared_ptr<Block> closed(const shared_ptr<Block> &block, set<string> &declared);
  shared_ptr<StmtBlock> closed(const shared_ptr<StmtBlock> &stmtBlock, set<string> declared);
  shared_ptr<LabledBlock> closed(const shared_ptr<LabledBlock> &LabledBlock, set<string> declared);

  bool has_error{false};
  Error err(const string msg);

  /// @brief check whether a program is closed
  /// @param program  error-free ast of program
  /// @return checked program
  variant<Program, bool> closed(Program &program)
  {
    has_error = false;
    set<string> declared{};

    list<Tdecl>& decls = program.getDecls();
    list<Tdecl> checkedDecls = closed(decls, declared);

    list<Tstmt> &stmts = program.getStmts();
    list<Tstmt> checkedStmts = closed(stmts, declared);

    shared_ptr<Expr> expr = get<shared_ptr<Expr>>(program.getExpr());
    Texpr checkedExpr = closed(expr, declared);
    
    if (has_error)
    {
      return has_error;
    }
    return Program(move(checkedDecls), move(checkedStmts), move(checkedExpr));
  }

  bool closed(Variable &var, set<string> &declared)
  {
    return declared.count(var.getName())>0;
  }

  Texpr closed(const shared_ptr<Expr> &expr, set<string> &declared)
  {

    if (shared_ptr<Number> num = dynamic_pointer_cast<Number>(expr))
    {
      return expr;
    }
    else if (shared_ptr<Variable> var = dynamic_pointer_cast<Variable>(expr))
    {
      if (closed(*var, declared))
      {
        return var;
      }
      return err("variable "+var->getName()+" is not defined");
    }
    else if (shared_ptr<BinOp> binOp = dynamic_pointer_cast<BinOp>(expr))
    {

      const string leftVar = binOp->left().getName();
      const string rightVar = binOp->right().getName();

      if (closed(binOp->left(), declared))
      {

        if (closed(binOp->right(), declared))
        {
          return binOp;
        }
        else
        {
          Error er = err("variable of "+binOp->right().getName()+" is not defined");
          return make_shared<BinOp>(binOp->op(), binOp->left(), move(er));
        }
      }
      else
      {
        if (closed(binOp->right(), declared))
        {
          Error er = err("variable  "+binOp->left().getName()+" is not defined");
          return make_shared<BinOp>(binOp->op(), move(er), binOp->right());
        }
        else
        {
          Error leftErr = err("variable "+binOp->left().getName()+" is not defined");
          Error rightErr = err("variable "+binOp->right().getName()+" is not defined");
          return make_shared<BinOp>(binOp->op(), move(leftErr), move(rightErr));
        }
      }
    }
    else
    {
      cout << "should not reach here: no such Expr" << endl;
      abort();
    }
  }

  list<Tdecl> closed(list<Tdecl> &decls, set<string> &declared)
  {
    list<Tdecl> checkedDeclList;
    for (auto &item : decls)
    {
      checkedDeclList.push_back(move(closed(item, declared)));
    }
    return checkedDeclList;
  }

  Tdecl closed(Tdecl &decl, set<string> &declared)
  {
    Declare &declRef = get<Declare>(decl);
    shared_ptr<Expr> expr = declRef.getExpr();

    Texpr checkedExpr = closed(expr, declared);
    declared.insert(declRef.getVar().getName());
    return Declare{move(declRef.getVar()), move(checkedExpr)};
  }

  list<Tstmt> closed(list<Tstmt> &stmts, set<string> declared)
  {
    list<Tstmt> checkedStmtList;
    for (auto &item : stmts)
    {
      shared_ptr<Statment> stmtPtr = closed(get<shared_ptr<Statment>>(item), declared);
      checkedStmtList.push_back(stmtPtr);
    }
    return checkedStmtList;
  }

  shared_ptr<Statment> closed(shared_ptr<Statment> &stmt, set<string> declared)
  {
    // if0, whil0, assign
    if (shared_ptr<Assign> ass = dynamic_pointer_cast<Assign>(stmt))
    {
      return closed(ass, declared);
    }
    else if (shared_ptr<If> if0 = dynamic_pointer_cast<If>(stmt))
    {
      return closed(if0, declared);
    }
    else if (shared_ptr<While> whil0 = dynamic_pointer_cast<While>(stmt))
    {
      return closed(whil0, declared);
    }
    else
    {
      cout << "should not reach here: no such statment" << endl;
      abort();
    }
  }

  shared_ptr<Assign> closed(const shared_ptr<Assign> &ass, set<string> declared)
  {
    Texpr expr = ass->getExpr();
    Texpr checkedExpr = closed(get<shared_ptr<Expr>>(expr), declared);

    string varName = ass->getVar().getName();
    if (declared.count(varName))
    {
      return make_shared<Assign>(ass->getVar(), move(checkedExpr));
    }
    else
    {
      Error er = err("variable  "+varName+" is not defined");
      return make_shared<Assign>(move(er), move(checkedExpr));
    }
  }

  shared_ptr<If> closed(const shared_ptr<If> &if0, set<string> declared)
  {
    Texpr test = if0->getTest();
    shared_ptr<Block> then = if0->getThen();
    shared_ptr<Block> els = if0->getElse();

    Texpr checkedExpr = closed(get<shared_ptr<Expr>>(test), declared);
    shared_ptr<Block> checkedThen = closed(then, declared);
    shared_ptr<Block> checkedElse = closed(els, declared);
    return make_shared<If>(move(checkedExpr), move(checkedThen), move(checkedElse));
  }

  shared_ptr<While> closed(const shared_ptr<While> &while0, set<string> declared)
  {
    Texpr test = while0->getTest();
    shared_ptr<Block> body = while0->getBody();

    Texpr checkedExpr = closed(get<shared_ptr<Expr>>(test), declared);
    shared_ptr<Block> checkedBody = closed(body, declared);
    return make_shared<While>(move(checkedExpr), move(checkedBody));
  }

  shared_ptr<Block> closed(const shared_ptr<Block> &block, set<string> &declared)
  {
    if (shared_ptr<StmtBlock> stmtBlock = dynamic_pointer_cast<StmtBlock>(block))
    {
      return closed(stmtBlock, declared);
    }
    else if (shared_ptr<LabledBlock> labledBlock = dynamic_pointer_cast<LabledBlock>(block))
    {
      return closed(labledBlock, declared);
    }
    else
    {
      cout << "should not reach here: no such Block" << endl;
      abort();
    }
  }

  shared_ptr<StmtBlock> closed(const shared_ptr<StmtBlock> &stmtBlock, set<string> declared)
  {
    shared_ptr<Statment> stmt = stmtBlock->getStatment();
    shared_ptr<Statment> checkedStmt = closed(stmt, declared);
    return make_shared<StmtBlock>(move(checkedStmt));
  }

  shared_ptr<LabledBlock> closed(const shared_ptr<LabledBlock> &labledBlock, set<string> declared)
  {
    list<Tdecl>& decls = labledBlock->getDeclares();
    list<Tstmt>& stmts = labledBlock->getStatments();
    list<Tdecl> checkedDecls = closed(decls, declared);
    list<Tstmt> checkedStmts = closed(stmts, declared);
    return make_shared<LabledBlock>(move(checkedDecls), move(checkedStmts));
  }

  Error err(const string msg)
  {
    has_error = true;
   return Error(move(nullptr), msg);
  }

}