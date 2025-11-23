#include <iostream>
#include "../src/sexp/Parser.hpp"
#include "../src/ast/AstParser.hpp"
#include "../src/csk/CskMachine.hpp"
#include <variant>

using namespace ast;
using namespace sexpr;
using namespace csk;

const string VALUE{"number"};
const string PARSE_ERR{"parser error"};
const string RUN_TIME_ERR{"run-time error"};

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

variant<float, string> run(Program &p)
{
  CskMachine c;
  return c.run(p);
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
    Program& prog = *(prog_ptr.get());
   
    Result result = run(prog);
    if (holds_alternative<string>(result))
    {
      cout << RUN_TIME_ERR;
    }
    else 
    {
      cout << VALUE;
    }
  }
  return 0;
}