/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Program.hpp"
namespace ast
{
  Program::Program(list<unique_ptr<AstNode>> stmts, unique_ptr<AstNode> expr)
  {
    this->stmts = move(stmts);
    this->expr = move(expr);
  }
}
