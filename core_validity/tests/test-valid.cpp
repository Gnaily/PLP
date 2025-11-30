#include <iostream>
#include "../src/sexp/Parser.hpp"
#include "../src/ast/AstParser.hpp"
#include "../src/validity/Validity.hpp"
#include <variant>

using namespace ast;
using namespace sexpr;
using namespace validity;

const string BELONGS{"belongs"};
const string PARSE_ERR{"parser error"};
const string VALIDITY_ERR{"undeclared variable error"};

variant<string, shared_ptr<Program>> parse(std::string &src)
{
  Parser sexpr_p{make_unique<string>(src)};
  unique_ptr<SimpleSexpr> ss = sexpr_p.parse();
  AstParser ast_p{move(ss)};
  pair<bool, variant<shared_ptr<Program>, Error>> result = ast_p.parse();
  if (result.first)
  {
    return PARSE_ERR;
  }
  else
  {
    shared_ptr<Program> p = get<shared_ptr<Program>>(result.second);
    return p;
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
  variant<string, shared_ptr<Program>> res = parse(src);
  if (holds_alternative<string>(res))
  {
    cout << get<string>(res);
  }
  else
  {
    shared_ptr<Program> prog_ptr = move(get<shared_ptr<Program>>(res));
    variant<Program, bool> checkResult = validity::closed(*prog_ptr);
    if (holds_alternative<bool>(checkResult))
    {
      cout << VALIDITY_ERR;
    }
    else 
    {
      cout << BELONGS;
    }
  }
  return 0;
}