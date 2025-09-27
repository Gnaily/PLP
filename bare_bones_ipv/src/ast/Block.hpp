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
#include <variant>
#include "AstNode.hpp"
#include "Error.hpp"
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
    unique_ptr<Statment> stmt;

  public:
    StmtBlock(unique_ptr<Statment> stmt);
  };

  class LabledBlock : public Block
  {
  private:
    list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts;

  public:
    LabledBlock(list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts);
  };
}
#endif