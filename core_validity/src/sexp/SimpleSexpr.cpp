/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */
#include "./SimpleSexpr.hpp"
#include <iostream>
namespace sexpr
{
  using namespace sexpr;
  using namespace std;
  // ------------------------ helper function declaration of Name--------------------------------
  bool isAlphabetical(const char &ch);

  // ------------------------ helper function declaration of Number--------------------------------
  
  bool isNoneSymNum(const string_view &str);
  bool isPartOfNum(const string_view &str);
  bool isNumSeq(const string_view &str);
  bool isNum(char ch);
  // ------------------------ helper function declaration of Number--------------------------------
  
  // ------------------------ classes implamentation--------------------------------
  Name::Name(string name):name(move(name))
  {
  };

  //the caller can only read name
  const string &Name::value()
  {
    string& ref = this->name;
    return ref;
  }

  bool Name::isName(const string &str)
  {
    int len = str.length();
    if (!(len > 0 && len <= 20))
    {
      return false;
    }

    for (size_t i = 0; i < KEY_WORDS.size(); i++)
    {
      if (KEY_WORDS[i].compare(str) == 0)
      {
        return true;
      }
    }

    string::const_iterator iter = str.begin();
    while (iter != str.end())
    {
      if (!isAlphabetical(*iter))
      {
        return false;
      }
      iter++;
    }
    return true;
  };

  Number::Number(float f):num(f)
  {

  }

  float Number::value()
  {
    return this->num;
  }

  bool Number::isNumber(const string_view& str)
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


  Seq::Seq():lst(std::list<unique_ptr<SimpleSexpr>>())
  {
  };

  void Seq::add(unique_ptr<SimpleSexpr> ss)
  {
    this->lst.push_back(move(ss));
  }

  list<unique_ptr<SimpleSexpr>>& Seq::getList()
  {
    list<unique_ptr<SimpleSexpr>>& ref = this->lst;
    return ref;
  }

  // --------------------------------------------------------------------------------------

  // ------------------------ helper function implmentation--------------------------------
  bool isAlphabetical(const char &ch)
  {
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122);
  }

  bool isNoneSymNum(const string_view &str)
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

  bool isPartOfNum(const string_view &str)
  {
    int len = str.length();
    const char& firstChar = str.at(0);
    if (!isNum(firstChar))
    {
      return false;
    }

    if (len == 1)
    {
      return true;
    }

    const char&  nextChar = str.at(1);
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

  bool isNumSeq(const string_view &str)
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

  
  bool isNum(char ch)
  {
    char zero {'0'};
    char nine {'9'};
    if (ch >= zero && ch <= nine)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}