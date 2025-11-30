/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "Block.hpp"
#include "Stmt.hpp"
namespace ast
{
  StmtBlock::StmtBlock(shared_ptr<Statment> stmt)
      : stmt(move(stmt))
  {
  }

  shared_ptr<Statment> StmtBlock::getStatment()
  {
    return this->stmt;
  }

  LabledBlock::LabledBlock(list<variant<Declare, Error>> decls, list<variant<shared_ptr<Statment>, Error>> stmts)
      : decls(move(decls)), stmts(move(stmts))
  {
  }

  list<variant<Declare, Error>>&  LabledBlock::getDeclares() 
  {
    return this->decls;
  }

  list<variant<shared_ptr<Statment>, Error>>& LabledBlock::getStatments()
  {
    return this->stmts;
  }
}