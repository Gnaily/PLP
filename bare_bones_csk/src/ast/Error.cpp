#include "Error.hpp"
namespace ast
{
  Error::Error(unique_ptr<sexpr::SimpleSexpr> src, string error_msg)
  :src(move(src)), error_msg(move(error_msg))
  {
  
  }
}