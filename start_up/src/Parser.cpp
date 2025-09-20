/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang All rights reserved.
 */
#include "Parser.h"
#include <iostream>
#include <array>
using namespace std;

bool LOG_ON = false;

void log(string& token, int& cur_position)
{
  if (!LOG_ON)
  {
    return;
  }
  cout << "\n"<<"token :" << token << ",current position:"<< cur_position << endl;
}

bool is_blank(char ch);

Parser::Parser(string &str)
{
  this->str = str;
  this->cur_pos = 0;
}

unique_ptr<SimpleSexpr> Parser::parse()
{

  if (!has_next_token())
  {
    cout << "parse error :" << "empty input" << endl;
    cout << "aborted!" << endl;
    abort();
  }

  string token = next_token();
  string begin_seq = "(";
  string end_seq = ")";
  if (Name::isName(token))
  {
    if (has_next_token())
    {
      cout << "parse error:" << token << "should not has other charactor followed" << endl;
      cout << "aborted!" << endl;
      abort();
    }
    return make_unique<Name>(token);
  }
  else if (Number::isNumber(token))
  {
    if (has_next_token())
    {
      cout << "parse error:" << token << "should not has other charactor followed" << endl;
      cout << "aborted!" << endl;
      abort();
    }
    float n = stof(token);
    return make_unique<Number>(n);
  }
  else if (begin_seq.compare(token) == 0)
  {
    return this->parse_seq();
  }
  else
  {
    cout << "parse error at position:" << this->cur_pos << endl;
    cout << "aborted!" << endl;
    abort();
  }
}

unique_ptr<Seq> Parser::parse_seq()
{
  string begin_seq = "(";
  string end_seq = ")";

  unique_ptr<Seq> seq_ptr = make_unique<Seq>();
  while (has_next_token())
  {
    // ( is followed by one of bellow:
    // 1. ( 
    // 2. name
    // 3. number
    // 4. )
    string token = next_token();
    if (begin_seq.compare(token) == 0)
    {
      unique_ptr<Seq> ss_ptr = parse_seq();
      seq_ptr->add(move(ss_ptr));
    }
    else if (end_seq.compare(token) == 0)
    {
      return seq_ptr;
    }
    else
    {
      if (Name::isName(token))
      {
        unique_ptr<Name> name = make_unique<Name>(token);
        seq_ptr->add(move(name));
      }
      else if (Number::isNumber(token))
      {
        unique_ptr<Number> name = make_unique<Number>(stof(token));
        seq_ptr->add(move(name));
      }
      else
      {
        cout << "parse error at token:" << token << endl;
        cout << "aborted!" << endl;
        abort();
      }
    }
  }
  cout << "parse error :" << "lack "<<end_seq<<" at " << this->cur_pos << endl;
  cout << "aborted!" << endl;
  abort();
}

bool Parser::has_next_token()
{
  int start = skip_white_space();

  if (start < str.length())
  {
    return true;
  }
  return false;
}

// performance is bad here ,due to it's return type
string Parser::next_token()
{
  int start = this->cur_pos;
  string token;
  for (size_t i = start; i < str.length(); i++)
  {
    char ch = str.at(i);
    if (is_blank(ch))
    {
      token =  str.substr(start, (i - start));
      forward(1);
      log(token, this->cur_pos);
      return token;
    }
    else if (ch == '(' || ch == ')')
    {
      if (start == i)
      {
        
        token = str.substr(start, 1);
        forward(1);
        log(token, this->cur_pos);
        return token;
      }
      else
      {
        token = str.substr(start, (i - start));
        log(token, this->cur_pos);
        return token;
      }
    }
    else
    {
      forward(1);
    }
  }
  token = str.substr(start, (str.length() - start) + 1);
  log(token, this->cur_pos);
  return token;
}

int Parser::skip_white_space()
{
  int cur = this->cur_pos;
  for (size_t i = cur; i < this->str.length(); i++)
  {
    if (is_blank(str.at(i)))
    {
      forward(1);
    }
    else
    {
      break;
    }
  }
  return this->cur_pos;
}

void Parser::forward(const int& n)
{
  this->cur_pos = this->cur_pos + n;
}

array<char, 4> blanks = {' ', '\t', '\r', '\n'};
bool is_blank(char ch)
{
  for (size_t i = 0; i < blanks.size(); i++)
  {
    if (blanks.at(i) == ch)
    {
      return true;
    }
  }
  return false;
}