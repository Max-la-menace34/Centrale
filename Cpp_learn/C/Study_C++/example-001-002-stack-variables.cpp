#include <iostream> 
#include "num.hpp"  

/*

  In C++, a variable is a piece of memory that is reserved somewhere
  in the system, when the program you have written actually runs (not
  when you write your code, of course).

  The C++ language gives you the opportunity to reserve-and-name such
  pieces of memory. This is variable declaration. 

  e.g : let us declare two memory pieces i and j, able to host an
  integer, in other words, two int variables :

  int i;
  int j;

  As we like to trace what actually happens, we will rather use "num"
  instead of "int". This type "num" behaves like an "int", but it
  reports many things on the output. It is not standard but home-made;
  it is defined in "nump.hpp" (do not read num.hpp for now). So the
  previous declaration lines, if we rather want to use the num type
  instead of int, are the following ones:

  num i;
  num j;

  There are 4 distinct memory areas available when a program runs :
  The text, the stack, the heap and the data. Hereafter, the variables
  that we will declare are taken in the stack. It is a memory stack
  since when a variable is declared, its memory space reserved for it
  is taken "on the top" of the previously declared memory (i.e. pushed
  over the top of stack). When memory is released (we will see how),
  it is taken from the top of the stack. It works as a LIFO (last
  in, first out): When a release occur, it releases the memory that
  was put last on the stack (i.e the memory is popped out from the
  stack). In the following sketch, nwh means "next write will be here".

   +---+                  +---+                 +---+                +---+                +---+
   | 8 |                  | 8 |                 | 8 |                | 8 |                | 8 | <- nwh
   +---+                  +---+                 +---+                +---+                +---+
   | 9 |                  | 9 | <- nwh          | 9 |                | 9 | <- nwh         | 5 |
   +---+                  +---+                 +---+                +---+                +---+
   | 5 | <- nwh           | 3 |                 | 3 | <- nwh         | 4 |                | 4 |
   +---+                  +---+                 +---+                +---+                +---+
   .   .                  .   .                 .   .                .   .                .   .
   .   .                  .   .                 .   .                .   .                .   .
  #######                #######               #######              #######              #######
  #STACK#  ---push 3-->  #STACK# ---pop (3)--> #STACK# ---push 4--> #STACK# ---push 5--> #STACK#  
  #######                #######               #######              #######              #######

  A bit of vocabulary now: Getting a new piece of memory so that it is
  available for you to store data is ***memory allocation***. Giving
  it back to the system since you do not need it anymore is called
  ***memory release*** or ***memory free***. So for the stack, push
  allocates, pop releases.

  So what we say here is that memory is allocated on the top of the
  stack by the variable declaration (push). This occurs inside a scope
  (i.e. {...<here>...}). The release of all the variables declared
  inside a scope is performed when the scope is closed (with '}')
  (pop).

  In C++, this happens internally, you do not see it on the
  terminal. For our pedagogical num type, you are notified of what
  happens to them. Push prints "+++ ...", and pop prints "---
  ...". From a computational point of view, such printings are
  useless... but it is good for understanding what happens, when you
  learn C/C++. This is why we have designed the num type in num.hpp
  (you do not need to read this file).

  Let us see pushes and pops.

*/

int main(int argc, char* argv[]) { // 0-level scope opening.
  fun_scope;

  rem("Let us declare two int");
  num a {"a"};     // Memory is allocated but is not initialized to a specific value.
  int b = 5; // Memory is allocated and initialized with value 5.
  rem("a and b have been put on the stack, but nothing is traced");

  ___;


  rem("Let us declare two num");
  
  num i {"i"}; 
  // The braces are used here for giving i a label "i" that will be
  // displayed in the traces. It is recommended, but not mandatory, to
  // have a label that is identical to the variable name in your
  // code. This syntax is not detailed further, consider that this is
  // how we give a display tag to a num variable at declaration
  // (i.e. allocation) time.

  num j {"j", 5}; // Allocation and initialization with 5.
  
  rem("Notice the trace of the two num allocations (i and j) on the stack just above.");

  // Let us create other variables in a subscope.
  { // 1-level scope opening.
    scope("1-level");

    num k {"k", 10};
    num l {"l", 11};

    std::cout << scope_indent << "k = " << k << ", " << "l = " << l << std::endl;

    // Let us open a new scope in that scope.
    { // 2-level scope opening.
      scope("2-level");
      num m {"m", 100};
      num n {"n", 101};
      num o {"o", 102};

      std::cout << scope_indent << "#########" << std::endl
		<< scope_indent << '#' << std::endl
		<< scope_indent << "# o = " << o << std::endl
		<< scope_indent << '#' << std::endl
		<< scope_indent << "#########" << std::endl;

      rem("Next is the closing of the scope, notice the release of m, n, o.");
    } // 2-level scope closed.

    rem("We are back in 1-level scope");

    // Let us allocate another variable...
    num p {"p"};
    rem("p is allocated.");
  } // 1-level scope closing.

  rem("We are back in 0-level");

  // i, j, a, b are remaining on the stack. They will be released at
  // the end of the main scope. You will only see at runtime the
  // release of i and j, since int (the type of a and b) as well as
  // other kind of non pedagogical type of variables do not trace
  // anything when their memory is released. But of course, the
  // popping occurs for a and b as for i and j.
  return 0;
} // 0-level scope closing.
