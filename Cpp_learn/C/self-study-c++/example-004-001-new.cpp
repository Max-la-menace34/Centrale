#include "num.hpp"

int main(int argc, char* argv[]) {
    num b("b",15);
  num* a_ptr = new num("a", 10);
  auto b_ptr = new num(*a_ptr);  // copy...
  //auto b_bptr = new b; cela ne marche pas =>  error: ‘b’ does not name a type

  {
    scope("Pointer affectation");
    auto c_ptr = a_ptr;
    c_ptr = b_ptr;
  } // No num values released here.

  {
    scope("Pointer free");
    auto c_ptr = a_ptr;
    delete c_ptr; 
  }

  // Uncomment this to avoid a memory leak.
  // delete b_ptr;

  // Keep this commented out in order to avoid double free errors.
  // delete a_ptr;

  return 0;
}