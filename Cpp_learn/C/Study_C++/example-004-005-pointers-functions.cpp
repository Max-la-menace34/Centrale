#include <iostream>
#include "num.hpp"

/*

  Passing pointers to functions.

  If an argument of a function is a pointer (i.e num*), it is copied
  as well, as we have seen previously. A pointer is an address, and is
  nothing more than an integer. So the integer value of the argument
  is copied and pushed over the stack, as we have seen before.

  Nevertheless, if this integer is the address of some object stored
  somewhere in memory, the integer allows for accessing to that
  object... thanks to the * operator (*ptr). So if an address is
  known outside a function, and passed as an argument so that it is
  available inside a function, both parts of the code can access
  ***the same object***, since both part know where it is in memory.

*/


// Let us declare 3 raz functions, that are designed so that they set
// their argument to 0. They could have been defined just here, but we
// will provide their definition after the main.

void raz_cpy(num);  // Won't do the job of resetting the variable.
void raz_ref(num&); // Will work, we have already seen that mechanism.
void raz_ptr(num*); // Will work as well, but syntax is not that nice.


int main(int argc, char* argv[]) {
  fun_scope;

  num i {"i", 5};
  num j {"j", 5};
  num k {"k", 5};

  ___;

  rem("raz by copy... a bad idea.");
  raz_cpy(i);
  std::cout << scope_indent << "i = " << i << std::endl;
  
  ___;

  rem("raz by reference.");
  raz_ref(j);
  std::cout << scope_indent << "j = " << j << std::endl;
  
  ___;

  rem("raz by pointer.");
  raz_ptr(&k);  /* The address of k needs to be passed as an
		   argument. raz_ptr(k) leads to a compiling error,
		   since k is a variable of type num whereas the
		   function requires a num*. Try it and see what such
		   compiling errors look like ! */
  std::cout << scope_indent << "k = " << k << std::endl;
  
  ___;


  return 0;
}

void raz_cpy(num x) {
  fun_scope;
  x = 0; // The ***copy*** of argument i (named x) is set to 0, not i.
}

void raz_ref(num& x) {
  fun_scope;
  x = 0; // Since the argument type is num&, x ***is*** j. 
}

void raz_ptr(num* x) {
  fun_scope;
  *x = 0; // Since x is a copy of the address of k (i.e. &k), *x is
	  // the memory at that address, i.e. k itself.
}


/*
  Conclusion :

  raz_cpy implements a argument passing by copy, it cannot be used to
  modify the argument, since only a copy of it is available. Argument
  passing by copy may not be a problem if the idea is only to read
  x. Nevertheless, if the type were not num but a very big data, as a
  100x100 matrix for example, copying it for only reading the values
  takes time, while the values are already there in memory, in the
  original matrix.

  raz_ref is ok. Even if you need only to read the argument, passing
  it by reference avoids a copy.

  raz_ptr ***is equivalent*** to raz_ref. It passes by copy the
  address, not the object. So the copy is the copy of an int. The
  drawback is clearly the syntax. At call time, you write raz(&k)
  instead of raz(k), and when you use the argument, you write *x
  instead of x.

  Indeed, before C++, passing arguments by reference was only
  available via pointers, as raz_ptr. References have been introduced
  in C++ as a syntactic sugar, but behind the scene, references are
  pointers as well. So raz_ref is only a syntactic convenience to
  write raz_ptr. References are pointers (with supplementary
  constraints that are not detailed here).

*/
