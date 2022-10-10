#include <iostream>
#include "num.hpp"

/*

  A function call, at runtime, behaves like a scope. It stacks
  variables (the arguments, the variable containing the result if
  needed, the local variables of the function), do the job, and
  unstack what have been stacked.


  When a function body calls another function, a new scope is opened
  within the current function scope, and so on.

  Check the traces, the execution may vary from this theoretical
  view... indeed, each compiler has its strategy for optimizing the
  use of the stack.

*/


num norm2(num, num);
num power(num, num);
num factorial(num);

/* 
   You will see that norm2 uses power. So power should be declared
   before norm2, so that norm2 can use it. Indeed, since we declare
   all function headers beforehand, any piece of code can "talk about"
   the function, i.e. the compiler knows how to stack arguments, even
   if the functions are not defined yet. Declaring the function
   headers prevents from wondering about the order of function
   definitions.
*/


int main(int argc, char* argv[]) {
  fun_scope;
  
  // Let us do a bit of user interface

  std::cout << scope_indent << "Enter n for norm, f for factorial, and press return : "
	    << std::flush; // forces the display. std::endl does it
			   // but here, we do not want a new line.
  char user_choice;
  std::cin >> user_choice; // std::cin is the keyboard input stream here.

  if(user_choice == 'n') {
    rem("We are computing norm2(3, 4).");
    num x   = {"x", 3};
    num y   = {"y", 4};
    ___;
    num res = norm2(x, y);
    std::cout << scope_indent << x << "^2 + " << y << "^2 = " << res << std::endl;
    ___;
  }
  else if(user_choice == 'f') {
    rem("We are computing factorial(5).");
    num n   = {"n", 5};
    ___;
    num res = factorial(n);
    std::cout << scope_indent << "factorial(" << n << ") = " << res << std::endl;
    ___;
  }
  else {
    rem("Bad selection, aborting.");
  }

  return 0;
}


num power(num x, num n) {
  fun_scope;
  num res     {"res",     1};
  for(num counter {"counter", 1}; counter <= n; ++counter)
    res *= x;
  return res;
}

num norm2(num a, num b) {
  fun_scope;
  num n = {"n", 2};
  return power(a, n) + power(b, n);
}

num factorial(num n) {
  fun_scope;
  num res = {"res", 1};
  if(n == 0)
    return res;
  else
    return n * factorial(n - 1);
}
