#include "num.hpp"

void f(const num& x) {
  scope("f(const num& x)");
  num i = x; // x is not considered as expiring.
}

void f(num&& x) {
  scope("f(num&& x)");
  num i = x; // x is not considered as expiring, in spite of the && type.
}

void g(const num& x) {
  scope("g(const num& x)");
  num i = std::move(x); // std::move leads to non expiring value since x is const.
}

void g(num&& x) {
  scope("g(num&& x)");
  num i = std::move(x); // std::move lead to expiring value, i is built by moving.
}

int main(int argc, char* argv[]) {
  num a {"a", 10};
  num b {"b", 11};
  num c {"c", 12};
  num d {"d", 13};

  f(a);            // calls f(const num& x)
  f(std::move(b)); // calls f(num&& x)
  g(c);            // calls g(const num& x)
  g(std::move(d)); // calls g(num&& x)
  
  return 0;
}