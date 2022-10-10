#include <iostream>
#include "num.hpp"

/* 

 Here, we address functions and function calls. When arguments are
 passed to a function, they are ***copied*** on the top of the
 stack. This is called passing arguments "by copy". There is another
 way to pass arguments: by reference. This will be addressed
 later. Nevertheless, passing by reference is nothing more that
 passing memory adresses by copy, so understanding this tutorial,
 where passing argument "by copy" is detailed, is crucial since it
 adresses the core of function calls.

*/


/*

  Calling a function consists of stacking copies of the arguments, and
  then jump to the function code in order to execute it, having the
  arguments available on the top of the stack. In order to prepare
  that jump, the compiler only needs to know what has to be stacked,
  what the function computes is not needed for preparing the
  jump. This is what function headers do: provide the compiler with
  the necessary informations for preparing the stack before jumping to
  the function call. Here is an example for the function power.

 */

num power(num, num);

/* Here, we tell the compiler that before jumping to the routine (the
   piece of code) named "power", the execution has to stack two nums
   (this is what "(num, num)" means). The execution also has to stack a
   room for the result of pow. This result occupies the size of a num
   as well. This is mentionned to the compiler by the type name "num"
   that is at the beginning of the line. Remember that the compiler needs 
   to know at compile time the size of the elements pushed on the stack. 
   The word "void" could have been used if the routine do not produce any 
   result.
*/


/* As we have the specification of how to call power, let us write a
   main that uses it. */

int main(int argc, char* argv[]) { 

  { // We use a scope for the first experiment.
    scope("Experiment #1");
    
    num a {"a", 2};
    num b {"b", 5};
    num c {"c"};
    
    ___;
    
    rem("Let us compute c = a^b");
    c = power(a, b); // This ***is*** an assignment.
    std::cout << scope_indent << "c = " << c << std::endl;
    ___;
  
  }
  
  { // The second experiment is identical, except that c is not
    // declared beforehand.
    scope("Experiment #2");
    
    num a {"a", 2};
    num b {"b", 5};
    
    ___;
    
    rem("Let us compute c = a^b");
    
    num c = power(a, b);
    // This ***is not*** an assignment but an initialization, even if
    // the = symbol is used. This could have been written :
    //   num c {power(a, b)};
    
    std::cout << scope_indent << "c = " << c << std::endl;

    /* You should notice from the traces that "c" is now the "res"
       variable used in the function. */
    ___;
  }

  // Compare line by line the traces of the two experiments !
  return 0;
}


/* Of course, this will work if we tell what process should be
   performed for computing power. This is the actual function
   definition. It can be done afterwards, i.e. after some previous
   code mention the call of the function. This is feasible since we
   have already declared the function header to the compiler. The next
   definition has to be in accordance with the previously declared
   header. */
num power(num x, num n) {
  fun_scope;

  rem("You may have noticed that arguments have just been");
  rem("copied to two new variables on the stack.");

  // We can use symbols x and n to name those two copies.

  ___;

  // Let us use another variable for the result.
  num res     {"res",     1};
  rem("Indeed, res is not really allocated here... read comments in the code.");
  ___;

  for(num counter {"counter", 1}; counter <= n; ++counter)
    res *= x;

  ___;

  // "return" exits the function, but it also tells the compiler that
  // the "res" variable do not need to be allocated, since the memory
  // room needed for it is already on the stack, at the "slot" where
  // the result of the function is expected to be.
  return res;
}

/*

 Nota : On the execution traces, "+++ res" appears when the line "num
 res {"res", 1};" is encountered. It suggests that the memory for the
 variable res is allocated in the stack at that time. Nevertheless,
 the above comment actually says that that memory has been stacked
 previously, at function call. This is a limitation of our tracing
 mechanism: we print stuff for num when the memory is initialized,
 since c++ offers tools to customize this stage. We used that tool so
 that num variables print "+++ ...." when they get initialized. As
 usually allocation and initialization occur immediately one after the
 other, tracing initialization is equivalent to tracing
 allocation. "return" is one of the very exceptional break of that
 rule, so you see the trace "+++" for res after (and not when) it has
 actually been allocated, since the allocation actually occurs at
 function call, when a' and b' are traced, and printing occurs
 aftewards, at initialization, after printing the horizontal rule.

 In C++, as explained further, you can override the default
 initializations. This is how we have implemented the printing of the
 "+++" traces for the num type. The function used for initialization
 should be called an "initializer". When the execution leads to
 allocate some memory for a variable, it gets the memory (from the
 space available on the top of the stack here) and calls the
 initializer immediately... in most cases (this is not true for
 return). In object oriented programming, such initializers are rather
 called "constructors", which is a bit confusing since constructors
 are actually applied only to initialize a piece of memory ***after***
 some process made it actually exist (or made it available, the memory
 exists already, it is the RAM electronic component you have in your
 computer).

 */
