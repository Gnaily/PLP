#include "Error.hpp"
namespace ast
{
  Error::Error(shared_ptr<sexpr::SimpleSexpr> src, const string &error_msg)
  {
    this->src = src;
    this->error_msg = error_msg;
  }
}