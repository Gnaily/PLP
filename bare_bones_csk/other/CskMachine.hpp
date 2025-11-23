#ifndef BARE_BONES_CSK_MACHINE_H
#define BARE_BONES_CSK_MACHINE_H
#include <variant>
#include "../ast/Program.hpp"

namespace csk {
  using namespace std;
  using namespace ast;

  class CskMachine
  {
  public:
    CskMachine();
    ~CskMachine();

    /**
     * consume a program and return an answer which is a number or an error message.
     */
    variant<float, string> run(Program& program);
  };
  
}
#endif