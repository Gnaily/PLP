/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Stmt.hpp"
#include "Block.hpp"

namespace ast
{
  Assign::Assign(unique_ptr<AstNode> var, unique_ptr<AstNode> expr)
  {
    this->var = move(var);
    this->expr = move(expr);
  }

  While::While(unique_ptr<AstNode> test, unique_ptr<AstNode> body)
  {
    this->test = move(test);
    this->body = move(body);
  }

  If::If(unique_ptr<AstNode> test, unique_ptr<AstNode> then_block, unique_ptr<AstNode> else_block)
  {
    this->test = move(test);
    this->then_block = move(then_block);
    this->else_block = move(else_block);
  }
}