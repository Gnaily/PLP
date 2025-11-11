#include <iostream>
#include "../src/ast/AstParser.hpp"
#include "../src/sexp/Parser.hpp"
#include <variant>
using namespace ast;
using namespace sexpr;
string parse(std::string &src)
{
 
  Parser sexpr_p{make_unique<string>(src)};
  unique_ptr<SimpleSexpr> ss = sexpr_p.parse();
  AstParser ast_p { move(ss) };
  
  pair<bool, variant<unique_ptr<Program>, unique_ptr<Error>>> program = ast_p.parse();
  if (program.first)
  {
    return "parser error";
  }
  else
  {
   return "belongs";
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
  std::string &src = input;
  string s = parse(src);
  cout<<s;
  return 0;
}