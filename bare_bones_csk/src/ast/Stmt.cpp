/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Stmt.hpp"
#include "Block.hpp"
namespace ast
{
  Assign::Assign(variant<Variable, Error> var,
                 variant<shared_ptr<Expr>, Error> expr)
      : var(move(var)), expr(move(expr))
  {
  }

  Variable &Assign::getVar()
  {
    return get<Variable>(this->var);
  }

  shared_ptr<Expr> Assign::getExpr()
  {
    return get<shared_ptr<Expr>>(this->expr);
  }

  If::If(variant<shared_ptr<Expr>, Error> test,
         variant<shared_ptr<Block>,  Error> then_block,
         variant<shared_ptr<Block>,  Error> else_block)
      : test(move(test)), then_block(move(then_block)), else_block(move(else_block))
  {
  }

  shared_ptr<Expr> If::getTest()
  {
    return get<shared_ptr<Expr>>(this->test);
  }

  shared_ptr<Block> If::getThen()
  {
    return get<shared_ptr<Block>>(this->then_block);
  }

  shared_ptr<Block> If::getElse()
  {
    return get<shared_ptr<Block>>(this->else_block);
  }

  While::While(variant<shared_ptr<Expr>,  Error> test,
               variant<shared_ptr<Block>, Error> body)
      : test(move(test)), body(move(body))
  {
  }

  shared_ptr<Expr> While::getTest()
  {
    return get<shared_ptr<Expr>>(this->test);
  }

  shared_ptr<Block> While::getBody()
  {
    return get<shared_ptr<Block>>(this->body);
  }
}