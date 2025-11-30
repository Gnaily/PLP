/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#ifndef BARE_BONES_AST_EXPR_H
#define BARE_BONES_AST_EXPR_H
#include <string>
#include <memory>
#include <variant>
#include "AstNode.hpp"
#include "Error.hpp"
namespace ast
{
  using std::string;
  using std::variant;
  enum EXPR_TYPE
  {
    NUMBER,
    VARIABLE,
    BINOP
  };

  class Expr : public AstNode
  {
  public:
    virtual EXPR_TYPE type() = 0;
    virtual ~Expr() {}
  };

  class Number : public Expr
  {
  private:
    float n;

  public:
    Number(float n);
    float getNumber();
    EXPR_TYPE type();
  };

  class Variable : public Expr
  {
  private:
    string name;

  public:
    Variable(string name);
    EXPR_TYPE type();

    const string getName();
  };

  class BinOp : public Expr
  {
  private:
    string oprt;
    variant<Variable, Error> leftVar;
    variant<Variable, Error> rightVar;

  public:
    BinOp(string oprt,
          variant<Variable, Error> leftVar,
          variant<Variable, Error> rightVar);
    EXPR_TYPE type();

    const string &op();
    Variable &left();
    Variable &right();
  };
}
#endif
