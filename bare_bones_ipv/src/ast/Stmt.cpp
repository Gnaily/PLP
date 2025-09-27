/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Stmt.hpp"
#include "Block.hpp"

namespace ast
{
  Assign::Assign(variant<unique_ptr<Variable>, unique_ptr<Error>> var,
                 variant<unique_ptr<Expr>, unique_ptr<Error>> expr)
  {
    this->var = move(var);
    this->expr = move(expr);
  }

  While::While(variant<unique_ptr<Expr>, unique_ptr<Error>> test,
               variant<unique_ptr<Block>, unique_ptr<Error>> body)
  {
    this->test = move(test);
    this->body = move(body);
  }

  If::If(variant<unique_ptr<Expr>, unique_ptr<Error>> test,
         variant<unique_ptr<Block>, unique_ptr<Error>> then_block,
         variant<unique_ptr<Block>, unique_ptr<Error>> else_block)
  {
    this->test = move(test);
    this->then_block = move(then_block);
    this->else_block = move(else_block);
  }
}