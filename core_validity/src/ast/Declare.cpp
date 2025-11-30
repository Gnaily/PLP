#include "Declare.hpp"
namespace ast
{
  Declare::Declare(variant<Variable, Error> var,
                   variant<shared_ptr<Expr>, Error> expr)
      : var(move(var)), expr(move(expr))
  {
  }

  Variable &Declare::getVar()
  {
    return get<Variable>(this->var);
  }

  shared_ptr<Expr> Declare::getExpr()
  {
    return get<shared_ptr<Expr>>(this->expr);
  }

}