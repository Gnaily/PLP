/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#ifndef BARE_BONES_AST_EXPR_H
#define BARE_BONES_AST_EXPR_H
#include <string>
#include <memory>
#include "AstNode.hpp"
namespace ast
{
  using namespace std;
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
    std::string name;

  public:
    Variable(const std::string &name);
    EXPR_TYPE type();
  };

  class BinOp : public Expr
  {
  private:
    std::string op;
    unique_ptr<AstNode> left_var;
    unique_ptr<AstNode> right_var;

  public:
    BinOp(string op, unique_ptr<AstNode> left, unique_ptr<AstNode> right);
    EXPR_TYPE type();
  };
}
#endif
