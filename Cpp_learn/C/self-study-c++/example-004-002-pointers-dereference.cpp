#include <iostream>
#include "num.hpp"

/*
  
  Let us do further manipulation with pointers.
 
 */


int main(int argc, char* argv[]) {
  fun_scope;

  // Let us declare a variable, and a "pointer to it".
  num      i = {"i", 3};
  num* ptr_i = &i;

  ___;
  
  rem("i = 5");
  i = 5;
  ___;

  // Since we have a pointer ptr_i with the address of i in it, we can
  // use it to retreive the stuff that is addressed (i.e. the variable
  // i). This is what operator * does.
  
  rem("print *ptr_i");
  std::cout << scope_indent << "Printing *ptr_i : " << *ptr_i << std::endl;

  // We can use *ptr_i, that is i, for any manipulation that we would have done with i.
  rem("num j = *ptr_i + *ptr_i");
  num j = *ptr_i + *ptr_i;
  
  // Even assignment works.
  rem("*ptr_i = 6 : compare with previous i = 5");
  *ptr_i = 6;
  ___;

  return 0;
}


/*
  There is a similar ambiguity with "*" that we have noticed
  previously with "&". Here, do not confuse pointer types with the
  operator that allows to get what is addressed (we say to
  ***dereference***) :
  
  int  i;
  int* ptr = &i; 
  *ptr = 0;

  */
