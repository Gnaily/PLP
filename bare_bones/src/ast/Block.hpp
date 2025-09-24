/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 *
 * Block is one of
 * - Statment
 * - (block Statment+)
 *
 * the Statment+ represent there is at least one Statment
 *
 */

#ifndef BARE_BONES_AST_BLOCK_H
#define BARE_BONES_AST_BLOCK_H
#include <list>
#include <memory>
#include "AstNode.hpp"

namespace ast
{
  using namespace std;
  class Statment;

  class Block : public AstNode
  {
  public:
    virtual ~Block() {};
  };

  class StmtBlock : public Block
  {
  private:
    unique_ptr<AstNode> stmt;

  public:
    StmtBlock(unique_ptr<AstNode> stmt);
  };

  class LabledBlock : public Block
  {
  private:
    list<unique_ptr<AstNode>> stmts;

  public:
    LabledBlock(list<unique_ptr<AstNode>> stmts);
  };
}
#endif