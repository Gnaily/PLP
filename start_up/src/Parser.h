/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang All rights reserved.
 */
#if !defined(START_UP_PARSER_H)
#define START_UP_PARSER_H
#include <memory>
#include "SimpleSexpr.h"
class Parser
{
private:
  std::string str;
  /**
   * point to the current position when moving on the str.
   */
  int cur_pos = 0;
public:
  Parser(std::string& str);
  ~Parser(){};
  std::unique_ptr<SimpleSexpr> parse();
private:
  std::unique_ptr<Seq> parse_seq();
  int skip_white_space();
  void forward(const int& n);
  bool has_next_token();
  std::string next_token(); 
};

#endif
