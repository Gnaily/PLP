/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Block.hpp"
#include "Stmt.hpp"
namespace ast
{
  StmtBlock::StmtBlock(unique_ptr<Statment> stmt)
  {
    this->stmt = move(stmt);
  }

  LabledBlock::LabledBlock(list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts)
  {
    this->stmts = move(stmts);
  }
}