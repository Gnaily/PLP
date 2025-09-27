/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#ifndef BARE_BONES_AST_ERROR_H
#define BARE_BONES_AST_ERROR_H
#include <string>
#include <memory>
#include "AstNode.hpp"
#include "../sexp/SimpleSexpr.hpp"
namespace ast
{
  using namespace std;

  class Error : public AstNode
  {
  private:
    unique_ptr<sexpr::SimpleSexpr> src;
    unique_ptr<string> error_msg;

  public:
    Error(unique_ptr<sexpr::SimpleSexpr> src, unique_ptr<string> error_msg);
  };
}
#endif
