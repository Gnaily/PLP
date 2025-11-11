/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Block.hpp"
namespace ast
{
  StmtBlock::StmtBlock(unique_ptr<AstNode> stmt)
  {
    this->stmt = move(stmt);
  }

  LabledBlock::LabledBlock(list<unique_ptr<AstNode>> stmts)
  {
    this->stmts = move(stmts);
  }
}