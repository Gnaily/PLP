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
  class Program : public AstNode
  {
  private:
    list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts;
    variant<unique_ptr<Expr>, unique_ptr<Error>> expr;

  public:
    Program(list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts,
            variant<unique_ptr<Expr>, unique_ptr<Error>> expr);
  };
}
#endif
