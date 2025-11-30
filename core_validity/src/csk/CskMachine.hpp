#ifndef BARE_BONES_CSK_MACHINE_H
#define BARE_BONES_CSK_MACHINE_H
#include <variant>
#include <string>
#include <map>
#include "../ast/Program.hpp"

namespace csk
{

  #define STORE(T) map<string, float> &T::store()\
  {\
    return this->stor;\
  };

  #define CONTINATION(T) Program &T::contination()\
  {\
    return this->kont;\
  }\

  using namespace std;
  using namespace ast;

  using Ctrl = variant<shared_ptr<Expr>, string>;
  using Result = variant<float, string>;

  const string SEARCH{"i am searching next instruction..."};

  class State
  {
  public:
    virtual bool isInitial();
    virtual bool isFinal();
    virtual ~State() = default;
  };

  class Initial : public State
  {
  private:
    string ctrl{SEARCH};
    map<string, float> stor;
    Program &kont;

  public:
    Initial(Program &kont);
    map<string, float> &store();
    Program &contination();
    bool isInitial() override ;
  };

  class Intermidate : public State
  {
  private:
    Ctrl ctrl;
    map<string, float> stor;
    Program &kont;

  public:
    Intermidate(Ctrl ctrl, map<string, float> stor, Program &kont);
    variant<shared_ptr<Expr>, string> &control();
    map<string, float> &store();
    Program &contination();
  };

  class Final : public State
  {
  private:
    Result ctrl;
    map<string, float> stor;
    Program &kont;

  public:
    Final(Result ctrl, map<string, float> stor, Program &kont);
    Result getResult();
    bool isFinal() override;
  };

  class CskMachine
  {
  public:
    CskMachine();

    /**
     * consume a program and return an answer which is a number or an error message.
     */
    variant<float, string> run(Program &program);
  };

}
#endif