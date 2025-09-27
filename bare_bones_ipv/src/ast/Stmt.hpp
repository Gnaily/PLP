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
#include <variant>
#include "Expr.hpp"
#include "Block.hpp"
#include "Error.hpp"
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
    variant<unique_ptr<Variable>, unique_ptr<Error>> var;
    variant<unique_ptr<Expr>, unique_ptr<Error>> expr;

  public:
    Assign(variant<unique_ptr<Variable>, unique_ptr<Error>> var,
           variant<unique_ptr<Expr>, unique_ptr<Error>> expr);
  };

  class If : public Statment
  {
  private:
    variant<unique_ptr<Expr>, unique_ptr<Error>> test;
    variant<unique_ptr<Block>, unique_ptr<Error>> then_block;
    variant<unique_ptr<Block>, unique_ptr<Error>> else_block;

  public:
    If(variant<unique_ptr<Expr>, unique_ptr<Error>> test,
       variant<unique_ptr<Block>, unique_ptr<Error>> then_block,
       variant<unique_ptr<Block>, unique_ptr<Error>> else_block);
  };

  class While : public Statment
  {
  private:
    variant<unique_ptr<Expr>, unique_ptr<Error>> test;
    variant<unique_ptr<Block>, unique_ptr<Error>> body;

  public:
    While(
        variant<unique_ptr<Expr>, unique_ptr<Error>> test,
        variant<unique_ptr<Block>, unique_ptr<Error>> body);
  };
}
#endif
