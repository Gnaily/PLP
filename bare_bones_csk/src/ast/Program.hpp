/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#ifndef BARE_BONES_AST_Block_H
#define BARE_BONES_AST_Block_H
#include <list>
#include <memory>
#include <variant>
#include "AstNode.hpp"
#include "Stmt.hpp"
#include "Error.hpp"
namespace ast
{
  //type alas
  using Tstmt = variant<shared_ptr<Statment>, Error>;
  using Texpr = variant<shared_ptr<Expr>, Error>;

  class Program : public AstNode
  {
  private:
    list<Tstmt> stmts;
    Texpr expr;

  public:
    Program(list<Tstmt> stmts, Texpr expr);

    list<Tstmt>& getStmts();
    Texpr& getExpr();
  };
}
#endif
