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
#include "Declare.hpp"
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
    shared_ptr<Statment> stmt;

  public:
    StmtBlock(shared_ptr<Statment> stmt);
    shared_ptr<Statment> getStatment();
  };

  class LabledBlock : public Block
  {
  private:
    list<variant<Declare, Error>> decls;
    list<variant<shared_ptr<Statment>, Error>> stmts;

  public:
    LabledBlock(list<variant<Declare, Error>> decls, list<variant<shared_ptr<Statment>, Error>> stmts);
    list<variant<Declare, Error>>&  getDeclares();
    list<variant<shared_ptr<Statment>, Error>>& getStatments();
  };
}
#endif