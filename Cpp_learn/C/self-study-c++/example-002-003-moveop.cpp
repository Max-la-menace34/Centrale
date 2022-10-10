#include "num.hpp"

// Operators +,* exist for type num, we use ours for the sake of
// illustration.

num plus(const num& a, const num& b)  {fun_scope; return {"res", (int)a + (int)b};}

num add(const num&  a, const num&  b) {fun_scope; return {"res", (int)a + (int)b};}
num add(      num&& a, const num&  b) {fun_scope; num res = std::move(a); res += b; return res;}
num add(const num&  a,       num&& b) {fun_scope; num res = std::move(b); res += a; return res;}
num add(      num&& a,       num&& b) {fun_scope; num res = std::move(a); res += b; return res;}


int main(int argc, char* argv[]) {
  {
    scope("Plus");
    num a {"a",    1};
    num b {"b",   10};
    num c {"c",  100};
    num d {"d", 1000};
    num res = plus(a, plus(b, plus(c, d)));
  }
  {
    scope("Add");
    num a {"a",    1};
    num b {"b",   10};
    num c {"c",  100};
    num d {"d", 1000};
    num res = add(a, add(b, add(c, d)));
  }

  return 0;
}