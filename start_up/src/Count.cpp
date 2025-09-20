/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */
#include <iostream>
#include "SimpleSexpr.h"
#include "Parser.h"
 
int countName(std::unique_ptr<SimpleSexpr> ss,  int& a)
{
  std::shared_ptr<SimpleSexpr> s_ss = std::move(ss);
  // use dynmic_cast is bad performance, but now it's not important.
  // here can also use the "int type()" method of the SimpleSexpr provided
  if (std::shared_ptr<Name> name = std::dynamic_pointer_cast<Name>(s_ss))
  {
    return a + 1;
  }
  else if (std::shared_ptr<Number> num = std::dynamic_pointer_cast<Number>(s_ss))
  {
     return a;
  }
  else if (std::shared_ptr<Seq> seq = std::dynamic_pointer_cast<Seq>(s_ss))
  {
    std::list<std::unique_ptr<SimpleSexpr>> l = seq->getList();
    for (auto &item : l)
    {
      int ret = countName(move(item), a);
      a = ret;
    }
    return a;
  }
  else
  {
    std::cout<< "error,expect a Name or a Number or a Seq" <<std::endl;
    abort();
  }
  
}

int main(int argc, char const *argv[])
{
  std::string input = "";
  std::string readed;
  while (std::getline(std::cin, readed))
  {
    input.append(readed).append("\n"); 
  }
  std::string& src = input;
  Parser p(src);
  int init_count = 0;
  int val = countName(p.parse(), init_count);
  std::cout<<val;
  return 0;
}