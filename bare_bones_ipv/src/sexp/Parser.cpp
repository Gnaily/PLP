/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */
#include "Parser.hpp"
#include <iostream>
#include <array>
namespace sexpr
{
  using namespace std;
  using namespace sexpr;
  bool LOG_ON = false;
  // ------------------------ helper functions declaration ------------------------
  void log(const string &token, const int &cur_position);
  bool is_blank(char ch);
  void skip_white_space(const string &str, int &cur);
  void forward(int &cur_pos, const int n);
  bool has_next_token(const string &str, int from);
  string next_token(const string &str, int &from);
  string &ref(string *str_ptr);
  unique_ptr<Seq> parse_seq(const string &str, int &from);
  //---------------------------------------------------------------------------------

  //------------------------------------class implementation-------------------------
  Parser::Parser(unique_ptr<string> str)
  {
    this->str = move(str);
    this->cur_pos = {0};
  }

  unique_ptr<SimpleSexpr> Parser::parse()
  {
    string &str_ref = ref(this->str.get());

    skip_white_space(str_ref, cur_pos);
    if (!has_next_token(str_ref, cur_pos))
    {
      cout << "parse error :" << "empty input" << endl;
      cout << "aborted!" << endl;
      abort();
    }
    
    string token = next_token(str_ref, cur_pos);
    
    string begin_seq = "(";
    string end_seq = ")";
    if (Name::isName(token))
    {
      skip_white_space(str_ref, cur_pos);
      if (has_next_token(str_ref, cur_pos))
      {
        cout << "parse error:" << token << " should not has other charactor followed" << endl;
        cout << "aborted!" << endl;
        abort();
      }
      unique_ptr<string> nm = make_unique<string>(token);
      return make_unique<Name>(move(nm));
    }
    else if (Number::isNumber(token))
    {
      skip_white_space(str_ref, cur_pos);
      if (has_next_token(str_ref, cur_pos))
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
      skip_white_space(str_ref, cur_pos);
      return parse_seq(str_ref, cur_pos);
    }
    else
    {
      cout << "parse error at position:" << this->cur_pos << endl;
      cout << "aborted!" << endl;
      abort();
    }
  }
  // ---------------------------------------------------------------------------------

  // ------------------------------helper function implementation---------------------
  unique_ptr<Seq> parse_seq(const string &str, int &from)
  {
    string begin_seq {"("};
    string end_seq = {")"};

    unique_ptr<Seq> seq_ptr = make_unique<Seq>();
    //skip_white_space(str, from);
    while (has_next_token(str, from))
    {
      // ( is followed by one of bellow:
      // 1. (
      // 2. name
      // 3. number
      // 4. )
     
      string token = next_token(str, from);
      if (begin_seq.compare(token) == 0)
      {
        unique_ptr<Seq> ss_ptr = parse_seq(str, from);
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
          unique_ptr<string> nm =make_unique<string>(token);
          unique_ptr<Name> name = make_unique<Name>(move(nm));
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
      skip_white_space(str, from);
    }
    cout << "parse error :" << "lack " << end_seq << " at " << from << endl;
    cout << "aborted!" << endl;
    abort();
  }

  bool has_next_token(const string &str, int start)
  {
    if (start < str.length())
    {
      return true;
    }
    return false;
  }

  string next_token(const string &str,  int &from)
  {
    int start = from;
    for (size_t i = from; i < str.length(); i++)
    {
      char ch = str.at(i);
      if (is_blank(ch))
      {
        const string token = str.substr(start, (i - start));
        forward(from, 1);
        log(token, from);
        return token;
      }
      else if (ch == '(' || ch == ')')
      {
        if (start == i)
        {

          const string token = str.substr(start, 1);
          forward(from, 1);
          log(token, from);
          return token;
        }
        else
        {
          const string token = str.substr(start, (i - start));
          log(token, from);
          return token;
        }
      }
      else
      {
        forward(from, 1);
      }
    }
    const string token = str.substr(start, (str.length() - start) + 1);
    log(token, from);
    return token;
  }

  void skip_white_space(const string &str, int &cur)
  {
    for (size_t i = cur; i < str.length(); i++)
    {
      char ch = str.at(i);
      if (!is_blank(ch))
      {
       return ;
      }
      forward(cur, 1);
    }
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

  void forward(int &cur_pos, int n)
  {
    cur_pos = cur_pos + n;
  }

  void log(const string &token, const int &cur_position)
  {
    if (!LOG_ON)
    {
      return;
    }
    cout << "\n"
         << "token :" << token << ",current position:" << cur_position << endl;
  }

  string &ref(string *str_ptr)
  {
    return *str_ptr;
  }
  // ---------------------------------------------------------------------------------
}
