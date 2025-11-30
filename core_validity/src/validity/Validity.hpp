#ifndef BARE_BONES_VALIDITY_H
#define BARE_BONES_VALIDITY_H
#include "../ast/Program.hpp"
namespace validity {
  using namespace ast;
  variant<Program, bool> closed(Program &program);
}
#endif