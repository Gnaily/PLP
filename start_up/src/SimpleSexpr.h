/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang All rights reserved.
 */

#if !defined(START_UP_SIMPLE_SEXPR_H)
#define START_UP_SIMPLE_SEXPR_H
#include <string>
#include <string_view>
#include <list>
#include<memory>
#define NAME  1
constexpr int NUMBER = 2;
constexpr int SEQ = 3;
class SimpleSexpr
{
public:
  virtual int type() = 0;
  virtual ~SimpleSexpr(){};
};

class Name : public SimpleSexpr
{
private:
  /* A Name is a non-empty sequence of at most 20 (US alphabetical) letters. */
  std::string name;
public:
  Name(std::string& name);
  ~Name(){};
  int type()
  {
    return 1;
  };
  static bool isName(std::string& str);

private:
  static bool isAlphabetical(char& ch);
};
 
class Number : public SimpleSexpr
{
private:
  /* A Number is a decimal number (contains a decimal point) 
  between -1000.0 and +1000.0, with at most one digit to the right 
  of the decimal point.
  */
  float num;

public:
  Number(float num);
  ~Number(){};
  int type()
  {
    return NUMBER;
  };
  static bool isNumber(std::string_view str);
private:
  static bool isNoneSymNum(const std::string_view& str);
  static bool isPartOfNum(const std::string_view& str);
  static bool isNumSeq(const std::string_view& str);
  static bool isNum(const char& ch);
  
};

class Seq : public SimpleSexpr
{
private:
  /* a list of sexpr */
  std::list<std::unique_ptr<SimpleSexpr>> list;
public:
  Seq();
  ~Seq(){};
  void add(std::unique_ptr<SimpleSexpr> ss);
  std::list<std::unique_ptr<SimpleSexpr>> getList();
  int type() 
  {
    return SEQ;
  };
  
};
#endif
