/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#ifndef BARE_BONES_AST_Block_H
#define BARE_BONES_AST_Block_H
#include <list>
#include <memory>
#include "AstNode.hpp"
#include "Stmt.hpp"
namespace ast
{
  class Program : public AstNode
  {
  private:
    list<unique_ptr<AstNode>> stmts;
    unique_ptr<AstNode> expr;

  public:
    Program(list<unique_ptr<AstNode>> stmts, unique_ptr<AstNode> expr);
  };
}
#endif
