#ifndef BARE_BONES_AST_DEF_H
#define BARE_BONES_AST_DEF_H
#include <string>
#include <memory>
#include "AstNode.hpp"
#include "Expr.hpp"
#include "../sexp/SimpleSexpr.hpp"
namespace ast
{
  using namespace std;

  class Declare : public AstNode
  {
  private:
    variant<Variable, Error> var;
    variant<shared_ptr<Expr>, Error> expr;

  public:
    Declare(variant<Variable, Error> var, variant<shared_ptr<Expr>, Error> expr);

    Variable &getVar();
    shared_ptr<Expr> getExpr();
  };
}
#endif