#include <iostream>
#include "num.hpp"  

/*

  Here, we introduce arrays... allocated in the stack.

*/

// This is a macro. Before actually compiling the code, g++ will
// replace every occurrence of SIZE in this file by 10. This is a
// pre-compiling action, transforming a cpp file into another cpp
// file, where macro are replaced by what they mean. This textual
// operation on ***your code*** has nothing to do with memory
// allocation, i.e. a macro definition is far from being a variable
// declaration.

#define SIZE 10  


int main(int argc, char* argv[]) {
  fun_scope;

  // The declaration just below allocates SIZE (i.e 10) pieces of
  // memory on the top of the stack. Note that 10 is defined in the
  // program code here. It is thus constant, and cannot be changed at
  // run time. For example, your program cannot get a size value from
  // the user and allocate that amount of nums onto the stack. The
  // size allocated on the stack must be known at compile time.
  num tab[SIZE];

  // If you wish to allocate an array whose size is only known at
  // execution, depending on the result of some computation or from
  // external input like user interface, you will have to allocate
  // your memory in the heap, and not in the stack. Such "size-defined
  // at runtime" allocation is exactly what the heap has been
  // designed for. This will be adressed in further tutorials.

  rem("tab allocated, let us start a for loop.");

  for(int i = 0; i < SIZE; ++i) {
    scope("for");
    num tmp {"tmp", 2*i};
    tab[i] = tmp; // The 10 variables are tab[0], tab[1], ... tab[9]
  }

  std::cout << scope_indent << std::endl
	    << scope_indent;
  for(int i = 0; i < SIZE; ++i)
    std::cout << tab[i] << ' ';
  std::cout << std::endl
	    << scope_indent << std::endl;
  
  rem("The loop is done. bye.");
  
  return 0;
}
