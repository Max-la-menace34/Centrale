
#include <iostream> 
#include "num.hpp"  


/*

  We will see few alternative for variable declaration ***in the
  stack***.

  We do it for int, and just after for our pedagogical num type. If
  the int variable is named "x", the num variable declared just after,
  similarly, will be named "_x". The '_' is a valid character for
  variable names.

*/



int main(int argc, char* argv[]) {
  fun_scope;
  
  // First, variables can be declared without any precision about the
  // value to be stored in the memory. This is the so-called "default"
  // allocation.

  rem("Default");
  int  i;
  num _i; // The label for display is set automatically and randomly
	  // as #<idf> (e.g #2039).

  // Variables can be both allocated and initialized at the same time.

  rem("Init from a value");
  int j = 5; // There are three ways...
  int k (5); // ... to write this...
  int l {5}; // ... but prefer the third.
  num _l {"l", 5};

  // Variables can be initialized as a copy of another variable of the
  // same kind. This is the so called "by copy" allocation.

  rem("Allocate by copying");
  int m = l;
  num _m = _l; // The label of _m will be "l'", i.e. the label of _l with a prime.
  std::cout << scope_indent << "_m = " << _m << std::endl;

  // This is an alternative notation of the allocation by copy.
  int n {l};
  num _n {_l}; // The label of _n will be "l'" as well, as for _m. _m
	       // and _n are two *** distinct *** num variables having
	       // the same label "l'" for display.
  std::cout << scope_indent << "_n = " << _n << std::endl;

  // Then, once allocation is done, the memory content of a variable
  // "a" can be set to be identical to the content of a variable
  // "b". This is assignment, denoted by "a=b". WARNING ! We have
  // used the '=' symbol previously, in allocation from a value and by
  // copy. When used in allocation, '=' IS NOT an assignment.

  rem("Assignment : m = a");
  
  int a = 23; // Not an assignment here
  m = a;      // Assignment here, m has been previously allocated.

  num _a {"a", 23};
  _m = _a;    // Assignment here, _m has been previously allocated.
  std::cout << scope_indent << "_m = " << _m << std::endl;
  
  rem("Releasing and quit");
  
  return 0;
}
