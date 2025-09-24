/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */
#ifndef BARE_BONES_AST_PARSER_H
#define BARE_BONES_AST_PARSER_H
#include <memory>
#include "../sexp/SimpleSexpr.hpp"
#include "AstNode.hpp"
#include "Program.hpp"
#include "Stmt.hpp"
#include "Block.hpp"
#include "Expr.hpp"
#include "Error.hpp"
namespace ast
{

  using namespace std;
  using namespace sexpr;

  
  class AstParser
  {
  private:
    unique_ptr<SimpleSexpr> ss;

  public:
    AstParser(unique_ptr<SimpleSexpr> ss);
    pair<bool, unique_ptr<AstNode>> parse();
  };
}
#endif