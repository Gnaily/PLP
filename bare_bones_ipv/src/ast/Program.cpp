/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Program.hpp"
namespace ast
{
  Program::Program(list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts,
                   variant<unique_ptr<Expr>, unique_ptr<Error>> expr)
  {
    this->stmts = move(stmts);
    this->expr = move(expr);
  }
}
