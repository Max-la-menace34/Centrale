#include <iostream>
#include "num.hpp"


/*

  Here, we introduce basic operations on nums.

*/

int main(int argc, char* argv[]) {
  fun_scope;

  num a {"a", 5};
  num b {"b", 3};
  num c {"c"};

  {
    scope("addition");

    rem("Computing c=a+b requires a temporary num for a+b");
    c = a + b; // This is an assignment
    rem("This temporary has been freed after having being copied.");
    ___;
  
    rem("But the compiler is clever...");
    num d = a + b;
    // This is copy allocation, NOT an assignment. d is thus directly
    // the num allocated for storing the a+b result. Using a temporary
    // result for the addition, and then allocating d in the stack so
    // that it has the value of the temporary result, and then remove
    // the temporary result, would have been a lost of time. The fact
    // that d is created right now allows for saving such operations.
    rem("No free if the newly allocated variable...");
    rem("... except when the scope closes.");
    ___;
  }

  {
    scope("increment");
    // This is pre increment. a is incremented, and the value of ++a is
    // the value of the variable ***after*** it has been incremented. So
    // "c = ++a" means increment a first, and then put its value into c.

    rem("c = ++a");
    a++;
    std::cout << scope_indent << "before : a = " << a << std::endl;
    c = ++a;
    std::cout << scope_indent << " after : a = " << a << std::endl;
  
    ___;


    // This is post increment. The variable a is incremented as well,
    // but the value of a++ is the value of a ***before*** it was
    // incremented. So "c = a++" means ok, increment a, but after its
    // value has been given to c. This requires a copy of a with the
    // value it had before increment. This is less efficient than ++a.
  
    rem("c = a++");
    std::cout << scope_indent << "before : a = " << a << std::endl;
    c = a++;
    std::cout << scope_indent << " after : a = " << a << std::endl;
  
    ___;

    rem("But the compiler is clever...");
    num e = a++;
  
    ___;
  }

  {
    scope("for loop");

    // Let us write a for loop, with a local variable, that sums 0, 1,
    // 2, ....9. The for loop in c++ is
    //
    //   for(INIT; TEST; INCREMENT) {
    //      ....
    //   }
    //
    // or
    //
    //   for(INIT; TEST; INCREMENT)
    //      <a single instruction>;
    //
    // or
    //
    //   for(INIT; TEST; INCREMENT); // if no instructions are necessary.
    //
    // This is the scheme of "for(A;B;C) D"
    //
    //          +-----+
    //          |  A  |
    //          +-----+
    //             |
    //             V
    //          +-----+  false
    //  +-----> | B ? |--------+
    //  |       +-----+        |
    //  |          | true      |
    //  |          V           |
    //  |       +-----+        |
    //  |       |  D  |        |
    //  |       +-----+        |
    //  |          |           |
    //  |          V           |
    //  |       +-----+        |
    //  |       |  C  |        |
    //  |       +-----+        |
    //  |          |           |
    //  +----------+           | 
    //                         |
    //             +-----------+
    //             |
    //             V
    //
  


    num res {"res", 0};
    for(num i {"i", 0}; i < 10; ++i /* ++i is faster than i++*/) {
      scope("for body");
      res += i;
    }
    std::cout << scope_indent << "res = " << res << std::endl;
  }

  rem("That's all folks !");
  
}
