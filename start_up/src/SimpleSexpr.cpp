/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang All rights reserved.
 */
#include "./SimpleSexpr.h"
#include <iostream>
using namespace std;
Name::Name(string &name)
{
  this->name = name;
};

bool Name::isName(string &str)
{
  int len = str.length();
  if (!(len > 0 && len <= 20))
  {
    return false;
  }

  string::iterator iter = str.begin();
  while (iter != str.end())
  {
    char ch = *iter;
    if (!isAlphabetical(ch))
    {
      return false;
    }
    iter++;
  }
  return true;
};

bool Name::isAlphabetical(char &ch)
{
  return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122);
};

Number::Number(float f)
{
  this->num = f;
}

bool Number::isNumber(string_view str)
{
  int len = str.length();
  if (len == 0 || len > 7)
  {
    return false;
  }

  char firstChar = str.at(0);
  if (firstChar == '-' || firstChar == '+')
  {
    return isNoneSymNum(str.substr(1, str.length()));
  }
  else
  {
    if (len > 6)
    {
      return false;
    }
    return isNoneSymNum(str);
  }
}

bool Number::isNoneSymNum(const string_view &str)
{
  int len = str.length();
  char firstChar = str.at(0);
  if (!isNum(firstChar))
  {
    return false;
  }

  if (len == 1)
  {
    return true;
  }

  if (firstChar == '0')
  {
    char nextChar = str.at(1);
    if (nextChar == '.')
    {
      int remain = len - 2;
      return remain > 0 ? isNumSeq(str.substr(2, len)) : false;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return isPartOfNum(str);
  }
}

bool Number::isPartOfNum(const string_view &str)
{
  int len = str.length();
  char firstChar = str.at(0);
  if (!isNum(firstChar))
  {
    return false;
  }

  if (len == 1)
  {
    return true;
  }

  char nextChar = str.at(1);
  if (nextChar == '.')
  {
    int remain = len - 2;
    return remain > 0 ? isNumSeq(str.substr(2, len)) : false;
  }
  else
  {
    return isPartOfNum(str.substr(1, len));
  }
}

bool Number::isNum(const char &ch)
{
  char zero = '0';
  char nine = '9';
  if (ch >= zero && ch <= nine)
  {
    return true;
  }
  else
  {
    return false;
  }
};

bool Number::isNumSeq(const string_view &str)
{
  int len = str.length();
  if (len == 0)
  {
    return false;
  }

  for (size_t i = 0; i < len; i++)
  {
    if (!isNum(str.at(i)))
    {
      return false;
    }
  }
  return true;
}

Seq::Seq()
{
  this->list = std::list<unique_ptr<SimpleSexpr>>();
};

void Seq::add(unique_ptr<SimpleSexpr> ss)
{
  this->list.push_back(move(ss));
}

list<unique_ptr<SimpleSexpr>> Seq::getList()
{
  return move(this->list);
}