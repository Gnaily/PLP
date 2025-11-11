#include "Error.hpp"
namespace ast
{
  Error::Error(unique_ptr<sexpr::SimpleSexpr> src, unique_ptr<string> error_msg)
  {
    this->src = move(src);
    this->error_msg = move(error_msg);
  }
}