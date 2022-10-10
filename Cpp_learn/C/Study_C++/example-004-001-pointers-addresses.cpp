#include <iostream>
#include <iomanip> // enables pretty prints. Here, std::setw(3) will display digits on 3 characters.
#include "num.hpp"

/*
  
  Here, we introduce... pointers. Scary isn't it ?

*/


int main(int argc, char* argv[]) {
  fun_scope;

  // Let us allocate a variable i in the stack.
  num i {"i", 24};

  // As every piece of memory in the world, at runtime, i occupies
  // some place somewhere. This place is the RAM of your
  // computer. Every available slot in the RAM allocated to your
  // process by the system is identified by an integer, that is its
  // address (denoted by @="at"). For example, the piece of memory at
  // address @237 is next to the piece @238, that is next to @239,
  // etc... In other words, the memory is a range of elementary boxes,
  // all labelled with consecutive integers. The label of a box is its
  // address.

  // To get the address of the box where i is stored, you can type
  // &i. &i is an integer, since it is a box number. Let us print it.
  std::cout << scope_indent << "The address of i is            " << &i << " (in hexadecimal here)." << std::endl;

  // Warning : we have already seen "&" in the type of function
  // arguments when they are passed by reference. Notations are very
  // similar, and thus confusing for beginners.
  //
  // num raz(num& arg) {...}         // Here & is used as a type suffix, for defining num&
  // std::cout << &i << std::endl;   // Here & is an operator so that &i is the address of i

  // We can store the address of i in another memory box. In this case,
  // the variable whose content is the address of some num has type
  // num*, that should be used at declaration time (as we used num and
  // int so far). As "int", "num", and many other, "num*" is a type.

  num* address_of_i {&i};
  // This could have been written
  //   num* address_of_i = &i;
  
  std::cout << scope_indent << "The address of i is            " << address_of_i << " (in hexadecimal here)." << std::endl;

  // We have taken two slots on the stack at this point. One for
  // storing the variable i, and another one for storing the variable
  // address_of_i. The first one contains something like {"i", 24},
  // the other one contains only an address, that is indeed the
  // address of the first box allocated in the stack (because we have
  // initialized address_of_i with &i).

  // You have to get used to the following drawing convention, and
  // make drawings in order to understand. Represent i with a box,
  // containing "i" and 24. This box is the chunk of memory where the
  // variable i is stored. The variable address_of_i is another box,
  // that you can draw as well. Its content is an integer, that you
  // cannot guess. What you know is that this content is the address
  // of the box you have drawn for i. Instead of writing an integer in
  // the address_of_i, draw an arrow that originates in the box
  // address_of_i and points to the i box. Here is an ascii art
  // example.
  //
  //              +-----+
  //              | "i" |
  //       +--->  +-----+  i
  //       |      | 24  |
  //       |      +-----+
  //       |    
  //       |      +-----+
  //       |      |     |
  //       +------+---  |  address_of_i
  //              |     |
  //              +-----+
  //
  //               STACK
  
  

  // Let us print the address of the second box, i.e. the address of
  // the address_of_i variable... by using & as well.

  std::cout << scope_indent << "The address of address_of_i is " << &address_of_i << " (in hexadecimal here)." << std::endl;

  // We can store the address of the second box... in a third box. Let
  // us declare a variable for this, with the right type (two *s).

  num** address_of_address_of_i = &address_of_i;
  std::cout << scope_indent << "The address of address_of_i is " << address_of_address_of_i << " (in hexadecimal here)." << std::endl;


  // You enjoyed ascii art ? Ok, let us draw address_of_address_of_i...
  //
  //              +-----+
  //              | "i" |
  //       +--->  +-----+  i
  //       |      | 24  |
  //       |      +-----+
  //       |    
  //       |      +-----+
  //       |      |     |
  //       +------+---  |  address_of_i
  //     +----->  |     |
  //     |        +-----+
  //     |      
  //     |        +-----+
  //     |        |     |
  //     +--------+---  |  address_of_address_of_i
  //              |     |
  //              +-----+
  //
  //               STACK
  
  

  // ... and so on.

  // Ok, we have three variables stacked. Let us add another one of
  // native type int, and another one containing its address.
  int j;
  int* address_of_j = &j;

  // We have now 5 variables on the stack. Let us see how many bytes of
  // memory each of them takes. The operator sizeof enables this.

  rem("Print variable sizes.");

  std::cout << scope_indent << "i                       (num)  : " << std::setw(3) <<  sizeof(i)                       << " bytes." << std::endl
	    << scope_indent << "address_of_i            (num*) : " << std::setw(3) <<  sizeof(address_of_i)            << " bytes." << std::endl
	    << scope_indent << "address_of_address_of_i (num**): " << std::setw(3) <<  sizeof(address_of_address_of_i) << " bytes." << std::endl
	    << scope_indent << "j                       (int)  : " << std::setw(3) <<  sizeof(j)                       << " bytes." << std::endl
	    << scope_indent << "address_of_j            (int*) : " << std::setw(3) <<  sizeof(address_of_j)            << " bytes." << std::endl;

  // At execution, you may have noted that (I comment what I see on my computer, adapt to yours if you see
  // other values) :
  // - int takes 4 bytes, i.e 4*8 = 32 bits.
  // - num is much bigger (40 bytes). Indeed, we store extra things in memory, as the name, to enable the traces.
  // - all addresses require 8 bytes, whatever the content of the box they refer to. 8x8=64 bits... I have a 64bit computer.

  // The computer does not need to check the memory at runtime to know
  // the size of the boxes. Indeed, the size is related to the
  // types. This is why you are asked to specify the type of the
  // variable at declaration time, so that the compiler knows how many
  // bytes need to be taken on the stack at runtime for storing that
  // variable !

  // As sizes are determined by the type, if you declare
  //     sometype x;
  // the size of x is sizeof(x) is determined by sizeof(sometype), that is always computable by the compiler
  // at compile time.
  
  rem("Print type sizes.");
  std::cout << scope_indent << "sizeof(num)    : " << std::setw(3) <<  sizeof(num)    << " bytes." << std::endl
	    << scope_indent << "sizeof(int)    : " << std::setw(3) <<  sizeof(int)    << " bytes." << std::endl
	    << scope_indent << "sizeof(int*)   : " << std::setw(3) <<  sizeof(int*)   << " bytes." << std::endl
	    << scope_indent << "sizeof(int**)  : " << std::setw(3) <<  sizeof(int**)  << " bytes." << std::endl
	    << scope_indent << "sizeof(num*)   : " << std::setw(3) <<  sizeof(num*)   << " bytes." << std::endl
	    << scope_indent << "sizeof(num***) : " << std::setw(3) <<  sizeof(num***) << " bytes." << std::endl;
  
  ___;

  // Not so scary isn't it ? We haven't mentionned "pointers" so
  // far. Indeed, a pointer is a variable whose content is the address
  // of something. That's it. All our address_of_<something> variables
  // here are pointers.
  
  return 0;
}

