#include <iostream>
#include "num.hpp"

/*
  
  Understand what assignment does on pointers.

  You can draw boxes and arrows as presented previously.
 
 */


int main(int argc, char* argv[]) {
  fun_scope;

  num i = {"i", 5};
  num j = {"j", 6};

  ___;

  num* ptr_i = &i;
  num* ptr_j = &j;
  std::cout << scope_indent << "i      = " << i << std::endl
	    << scope_indent << "j      = " << j << std::endl
	    << scope_indent << "ptr_i  = " << ptr_i << std::endl
	    << scope_indent << "ptr_j  = " << ptr_j << std::endl
	    << scope_indent << "*ptr_i = " << *ptr_i << std::endl
	    << scope_indent << "*ptr_j = " << *ptr_j << std::endl;
  ___;

  rem("ptr_j = ptr_i");
  ptr_j = ptr_i;
  ___;
  
  std::cout << scope_indent << "i      = " << i << std::endl
	    << scope_indent << "j      = " << j << std::endl
	    << scope_indent << "ptr_i  = " << ptr_i << std::endl
	    << scope_indent << "ptr_j  = " << ptr_j << std::endl
	    << scope_indent << "*ptr_i = " << *ptr_i << std::endl
	    << scope_indent << "*ptr_j = " << *ptr_j << std::endl;

  // We have only changed the address in ptr_j, i and j are not
  // modified.


  return 0;
}
