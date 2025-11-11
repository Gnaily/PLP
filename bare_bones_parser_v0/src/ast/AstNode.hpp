/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */
#if !defined(BARE_BONES_AST_NODE_H)
#define BARE_BONES_AST_NODE_H
#include <string>
namespace ast
{
  using namespace std;
  class AstNode
  {
  public:
    virtual ~AstNode() {};
  };
}

#endif
