/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#if !defined(START_UP_SIMPLE_SEXPR_H)
#define START_UP_SIMPLE_SEXPR_H
#include <string>
#include <string_view>
#include <list>
#include<memory>
namespace sexpr
{
using std::string;
using std::string_view;
using std::array;
using std::list;
using std::unique_ptr;

constexpr int NAME{1}; 
constexpr int NUM{2};
constexpr int SEQ{3};

constexpr string_view KEY_WORD_ADD{"+"};
constexpr string_view KEY_WORD_DIVID{"/"};
constexpr string_view KEY_WORD_COMPARE{"=="};
constexpr string_view KEY_WORD_ASSIGN{"="};
constexpr string_view KEY_WORD_IF{"if0"};
constexpr string_view KEY_WORD_WHILE{"while0"};
constexpr string_view KEY_WORD_BLOCK{"block"};
constexpr array<string_view ,7> KEY_WORDS{KEY_WORD_ASSIGN, KEY_WORD_ADD,KEY_WORD_DIVID,
   KEY_WORD_COMPARE, KEY_WORD_IF,KEY_WORD_WHILE, KEY_WORD_BLOCK};

class SimpleSexpr
{
public:
  virtual int type() = 0;
  virtual ~SimpleSexpr(){};
};

class Name : public SimpleSexpr
{
private:
  /* A Name is a non-empty sequence of at most 20 (US alphabetical) letters,and plus keywords */
  string name;
public:
  Name(string name);
  const string& value();
  int type()
  {
    return NAME;
  };
  static bool isName(const string& str);
};
 
class Number : public SimpleSexpr
{
private:
  /* A Number is a decimal number (contains a decimal point) 
  between -9999.9 and +9999.9, with at most one digit to the right 
  of the decimal point.
  */
  float num;

public:
  Number(float num);
  float value();
  int type()
  {
    return NUM;
  };
  static bool isNumber(const string_view& str);
};

class Seq : public SimpleSexpr
{
private:
  /* a list of sexpr */
  list<unique_ptr<SimpleSexpr>> lst;
public:
  Seq();
  ~Seq(){};
  void add(unique_ptr<SimpleSexpr> ss);
  list<unique_ptr<SimpleSexpr>>& getList();
  int type() 
  {
    return SEQ;
  };
  
};
}
#endif
