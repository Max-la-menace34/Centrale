#include <iostream>
#include "num.hpp"

/*

  Passing by value and by reference.

*/

/*

  We have seen so far that arguments are copied when they are passed
  to a function. For data occupying a lot amount of memory, this copy
  can take a while !  Let us see how to remove this constraint thanks
  to reference types (with the & symbol).

*/

/*
  This function resets two variables. It is implemented with a
  reference type for one argument, and without for the other, so that
  you can see what happens in both cases. A function that modifies its
  arguments does not need to compute some particular result. This is why
  it has no "return" statement and "void" is used for the
  return type. A function with a void return type is usefull only when
  it has a "side effect". Here, the side effect is modifying its
  arguments. Some very frequent side effects are modifying the screen,
  when you print something.
*/

void double_raz(num& x, num y) {
  fun_scope;
  
  x = 0;
  y = 0;
}


int main(int argc, char* argv[]) {
  fun_scope;

  num a {"a", 10};
  num b {"b", 10};

  std::cout << scope_indent << "(a, b) = (" << a << ", " << b << ')' << std::endl; 
  ___;
  double_raz(a, b);
  std::cout << scope_indent << "(a, b) = (" << a << ", " << b << ')' << std::endl; 
  ___;
  
  return 0;
}


/*

  The references (num&) are a syntactic sugar, i.e. convenient ways to
  describe your code, for hiding the fact that what is passed as an
  argument is not the num value (by copy), but the address in memory
  of the object. This leads us to... pointers ! They are commonly used
  in C++, sometimes directly, sometimes via reference types, where
  they are hidden by the syntax. Let us make this clear in the next
  tutorials.

 */
