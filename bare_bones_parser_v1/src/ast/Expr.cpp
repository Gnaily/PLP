/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 *
 * Expr is one of:
 * - Number
 * - Variable
 * - (Variable +  Variable)
 * - (Variable -  Variable)
 * - (Variable /  Variable)
 */

#include "Expr.hpp"
namespace ast
{

  Number::Number(float n)
  {
    this->n = n;
  }

  EXPR_TYPE Number::type()
  {
    return NUMBER;
  }

  Variable::Variable(unique_ptr<string> name)
  {
    this->name = move(name);
  }

  EXPR_TYPE Variable::type()
  {
    return VARIABLE;
  }

  BinOp::BinOp(unique_ptr<string> op,
               variant<unique_ptr<Variable>, unique_ptr<Error>> left,
               variant<unique_ptr<Variable>, unique_ptr<Error>> right)
  {
    this->op = move(op);
    this->left_var = move(left);
    this->right_var = move(right);
  }

  EXPR_TYPE BinOp::type()
  {
    return BINOP;
  }
}