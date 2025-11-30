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

  Number::Number(float n) : n(n)
  {
  }

  float Number::getNumber()
  {
    return this->n;
  }

  EXPR_TYPE Number::type()
  {
    return NUMBER;
  }

  Variable::Variable(string name) : name(name)
  {
  }

  const string Variable::getName()
  {
    return this->name;
  }

  EXPR_TYPE Variable::type()
  {
    return VARIABLE;
  }

  BinOp::BinOp(string oprt,
               variant<Variable, Error> leftVar,
               variant<Variable, Error> rightVar)
      : oprt(oprt), leftVar(move(leftVar)), rightVar(move(rightVar))
  {
  }

  EXPR_TYPE BinOp::type()
  {
    return BINOP;
  }

  const string &BinOp::op()
  {
    return this->oprt;
  }

  Variable &BinOp::left()
  {
    return get<Variable>(this->leftVar);
  }

  Variable &BinOp::right()
  {
    return get<Variable>(this->rightVar);
  }
}