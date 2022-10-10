// example-002-001-move.cpp
#include "num.hpp"

int main(int argc, char* argv[]) {
  num a {"a", 10};
  std::cout << &a << std::endl;
  num b {"b", 11};
  
  ___;

  rem("This does nothing to a.");
  std::cout << scope_indent << "a = " << std::move(a) << std::endl;
  ___;
  rem("This does nothing to a.");
  num c = a; // Usual copy
  rem("This alterates a and b.");

  num aa = std::move(a); // Move construction
  std::cout << &aa << std::endl;
  c      = std::move(b); // Move affectation
  ___;
  
  return 0;
}