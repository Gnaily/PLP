/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Program.hpp"
namespace ast
{
  Program::Program(list<Tdecl> decls, list<Tstmt> stmts, Texpr expr)
      : decls(move(decls)), stmts(move(stmts)), expr(move(expr)) {
        };

  list<Tdecl> &Program::getDecls()
  {
    return this->decls;
  }

  list<Tstmt> &Program::getStmts()
  {
    return this->stmts;
  }

  Texpr &Program::getExpr()
  {
    return this->expr;
  }
}
