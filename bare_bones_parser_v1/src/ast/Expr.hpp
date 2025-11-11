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
    EXPR_TYPE type();
  };

  class Variable : public Expr
  {
  private:
    unique_ptr<string> name;

  public:
    Variable(unique_ptr<string> name);
    EXPR_TYPE type();
  };

  class BinOp : public Expr
  {
  private:
    unique_ptr<string> op;
    variant<unique_ptr<Variable>, unique_ptr<Error>> left_var;
    variant<unique_ptr<Variable>, unique_ptr<Error>> right_var;

  public:
    BinOp(unique_ptr<string> op,
          variant<unique_ptr<Variable>, unique_ptr<Error>> left_var,
          variant<unique_ptr<Variable>, unique_ptr<Error>> right_var);
    EXPR_TYPE type();
  };
}
#endif
