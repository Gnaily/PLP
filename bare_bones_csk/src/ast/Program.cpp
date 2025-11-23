/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Program.hpp"
namespace ast
{
  Program::Program(list<Tstmt> stmts,
                   Texpr expr)
  {
    this->stmts = move(stmts);
    this->expr = move(expr);
  }

  list<Tstmt>& Program::getStmts()
  {
    return this->stmts;
  }

  Texpr& Program::getExpr()
  {
    return this->expr;
  }
}
