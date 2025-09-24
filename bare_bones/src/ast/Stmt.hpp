/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 *
 * Statment is one of:
 * - (Variable Expr)
 * - (If0 Expr Block Block)
 * - (While0 Expr Block)
 */

#ifndef BARE_BONES_AST_STMT_H
#define BARE_BONES_AST_STMT_H
#include <string>
#include <memory>
#include "Expr.hpp"
#include "Block.hpp"

namespace ast
{
  
  class Statment : public AstNode
  {
  public:
    virtual ~Statment() {};
  };

  class Assign : public Statment
  {
  private:
    unique_ptr<AstNode> var;
    unique_ptr<AstNode> expr;

  public:
    Assign(unique_ptr<AstNode> var, unique_ptr<AstNode> expr);
  };

  class If : public Statment
  {
  private:
    unique_ptr<AstNode> test;
    unique_ptr<AstNode> then_block;
    unique_ptr<AstNode> else_block;

  public:
    If(unique_ptr<AstNode> test, unique_ptr<AstNode> then_block, unique_ptr<AstNode> else_block);
  };

  class While : public Statment
  {
  private:
    unique_ptr<AstNode> test;
    unique_ptr<AstNode> body;

  public:
    While(unique_ptr<AstNode> test, unique_ptr<AstNode> body);
  };
}
#endif
