#include <iostream>  // Allows for writing to the terminal... and much more.
#include "num.hpp"   // This is home made tutorial stuff, not standard at all.
#include<stdio.h>
/*
  This is a C++ program. 
 
  In the following, 

  - fun_scope; 
  - scope;
  - rem("some message in natural human language");
  - ___;
  - std::cout << scope_indent << "blabla" << std::endl;

  are pedagogical tools provided by num.hpp in order to trace what
  happens in execution. This is not standard C++. You can remove them,
  everything will work fine but you won't have the traces. The
  following code does nothing else than calling these pedagogical
  tools.
*/

// Execution starts at ***the*** main function. Ignore the "int argc,
// char* argv[]" confusing stuff for the moment....

int main(int argc, char* argv[]) { // Symbol '{' opens the "scope" of main.
  
  // fun_scope shows "--- main" now, and "--- (main)" at the end of
  // the scope. A scope is a brace-enclosed piece of code. Here, the
  // name displayed (i.e. main) is the name of the current function we
  // are in. The function name "main" is reserved for the function the
  // execution enters in at start.
  fun_scope;

  // This is a remark nicely printed on the terminal.
  rem("This is a smart remark, isn't it ?");

  // This prints a bar on the terminal.
  ___;

  // Let us open a scope within the main scope
  {
    // We trace this scope on the output. As opposed to fun_scope, we
    // give explicitly the message displayed.
    scope("Internal scope"); // This writes "--- Internal scope"
    
    rem("I will print \"Hello world\" hereafter");

    // This is how output is made in c++ (we have included iostream
    // for that). std::cout is the name of the "output stream" of
    // charaters, i.e. the terminal. You can write several things by
    // using the "<<" operator. The last element we write in the
    // stream, "std::endl", means "end of line". It also ***flushes***
    // the output. Indeed, every characters you print are not written
    // immediately on the terminal. They are stored in a buffer and
    // printing actally occurs when the buffer is full. This saves
    // time at the system level. Here, the few characters we write are
    // not numerous enough to fill the buffer, so we have to force the
    // buffer to flush its content to the terminal. std::endl does
    // this just after having added a newline. You can also use
    // "std::flush" to force a buffer flush when you want to, without
    // adding a newline.
    std::cout << scope_indent << "Hello world"<< std::endl;
    std::cout << scope_indent << "Test" << std::endl;
    rem("The printing is done. That's all for this scope");
    
  } // The scope is closed here. As we used "scope("Internal scope");"
    // at the opening, the closing writes "--- (Internal scope)".

  return 0; // The main function returns an error code. 0 means "ok".
} // Symbol '}' closes the scope of main. As we used "fun_scope;" at
  // the opening, the closing writes "--- (main)".
