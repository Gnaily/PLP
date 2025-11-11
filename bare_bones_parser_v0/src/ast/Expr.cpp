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

  Variable::Variable(const std::string &name)
  {
    this->name = name;
  }

  EXPR_TYPE Variable::type()
  {
    return VARIABLE;
  }

  BinOp::BinOp(string op, unique_ptr<AstNode> left, unique_ptr<AstNode> right)
  {
    this->op = op;
    this->left_var = move(left);
    this->right_var = move(right);
  }

  EXPR_TYPE BinOp::type()
  {
    return BINOP;
  }
}