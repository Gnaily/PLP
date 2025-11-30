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
    variant<Variable, Error> var;
    variant<shared_ptr<Expr>, Error> expr;

  public:
    Assign(variant<Variable, Error> var, variant<shared_ptr<Expr>, Error> expr);

    Variable &getVar();
    shared_ptr<Expr> getExpr();
  };

  class If : public Statment
  {
  private:
    variant<shared_ptr<Expr>, Error> test;
    variant<shared_ptr<Block>, Error> then_block;
    variant<shared_ptr<Block>, Error> else_block;

  public:
    If(variant<shared_ptr<Expr>,  Error> test,
       variant<shared_ptr<Block>,  Error> then_block,
       variant<shared_ptr<Block>,  Error> else_block);
    shared_ptr<Expr> getTest();
    shared_ptr<Block> getThen();
    shared_ptr<Block> getElse();
  };

  class While : public Statment
  {
  private:
    variant<shared_ptr<Expr>, Error> test;
    variant<shared_ptr<Block>, Error> body;

  public:
    While(
        variant<shared_ptr<Expr>, Error> test,
        variant<shared_ptr<Block>, Error> body);
    shared_ptr<Expr> getTest();
    shared_ptr<Block> getBody();
  };
}
#endif
