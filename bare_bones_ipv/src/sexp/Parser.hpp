/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */
#if !defined(START_UP_PARSER_H)
#define START_UP_PARSER_H
#include <memory>
#include "SimpleSexpr.hpp"
namespace sexpr
{
  using namespace sexpr;
  
  class Parser
  {
  private:
    unique_ptr<string> str;
    /**
     * point to the current position when moving on the str.
     */
    int cur_pos;

  public:
    Parser(unique_ptr<string> str);
    ~Parser() {};
    unique_ptr<SimpleSexpr> parse();
  };
};
#endif
