#include <iostream>
#include "num.hpp"

/*

  Here, we learn how to use a memory zone that is not the stack we
  used so far: the heap ! Keep in mind that the amount of memory
  needed in the stack is determined at compiling time. For example, if
  you declare two variables, seeing the code is enough to deduce that
  two slots for these variables will be required on the top of the
  stack. If you write a code that calls a function with two arguments
  that returns a value, you can predict that this function call, at
  runtime, will stack the two arguments and the slot for the result:
  nothing more, nothing less. Such allocations whose size can be
  determined by reading the code only, i.e. at compiling time for g++,
  is called ***static allocation***.

  Now, let us suppose that you ask the user for a number n, and you
  want to allocate a n-sized array such as tab[i] = i, 0 <= i < n
  (warning, 0 is included, n excluded). In the code, there will be
  instructions for getting n from the keyboard, but the value of n
  entered by the user, that will be the amount of memory in the array,
  depends on what the user enters. The variable n can be 10, 100,
  1000... or whatever the user wants. Looking at the code cannot tell
  you the amount of memory, this will be determined at runtime. Such
  allocation is a ***dynamic allocation***: this is what the heap has
  been designed for.

 */

int main(int argc, char* argv[]) {
  fun_scope;

  num i;           
  num j {"j", 26}; 

  num* ptr = &i; // We set the pointer to be the address of i;
  std::cout << scope_indent << "ptr = " << ptr << ", *ptr = " << *ptr << std::endl;
  ptr = &j; // We set the pointer to be the address of j;
  std::cout << scope_indent << "ptr = " << ptr << ", *ptr = " << *ptr << std::endl;

  // ok, nothing new so far, i and j are taken in the stack, as
  // variables are.

  ___;

  ptr = new num;
  // new num allocates a piece of memory ***in the heap***, big enough
  // since it has sizeof(num) bytes, and returns the address of that
  // piece of memory. You can see in the trace that default
  // initialization has been used to initialize this piece of
  // memory. As "new num" is an address, i.e. an integer, it can be
  // stored in a pointer variable, ptr here. The "box" ptr, that has
  // the size for an address, is in the stack. What is in the heap is
  // the memory available at this address.
  //
  //  
  //       STACK                HEAP
  //
  //      |     |            
  //      |     |             |     |
  //      +-----+             |     |
  //      |     |             +-----+  --+
  // ptr  |  ---+---------->  |     |    |
  //      |     |             |     |    |
  //      +-----+             |     |    |
  //      |     |             |     |    | sizeof(num) bytes.
  //      |     |             |     |    |
  //                          |     |    |
  //                          |     |    |
  //                          +-----+  --+
  //                          |     |
  //                          |     |
  
  
 
  rem("ptr = new num;");
  std::cout << scope_indent << "ptr = " << ptr << ", *ptr = " << *ptr << std::endl;
  // ptr works as previously, *ptr is a num in the heap. Indeed, once
  // you have an address in a pointer variable, the way you can use it
  // does not differ according to the location of the memory at this
  // address (stack or heap).

  ___;

  // If some type supports several kind of initializition, i.e. more
  // than the default one, you can initialize the memory reserved in
  // the heep by new with custom initialization. Our num type supports
  // {"k", 12} as initialization, we can thus write the following.
  ptr = new num("k", 12);
  rem("ptr = new num(\"k\", 12);");
  std::cout << scope_indent << "ptr = " << ptr << ", *ptr = " << *ptr << std::endl;
 
  ___;


  // Last, we can ask for an array of num in the heap. They will be
  // contiguous, so the iterator-like pointer manipulation we have
  // presented for arrays in the stack work the same way. Here, new is
  // the address of the first element of the array, as tab is when you
  // allocate the array in the stack with "num tab[SIZE];".

  // With the heap, we can allocate the array dynamically. When I
  // wrote this code, I could not guess the size the user will ask.
  rem("ptr = new num[array_size];");
  int array_size;
  std::cout << scope_indent << "Enter the size of the array : " << std::flush;
  std::cin >> array_size;
  ptr = new num[array_size]; // This allocates array_size contiguous nums in the heap.
  rem("The array is allocated, and initialized by default.");
  num* iter = ptr;
  for(int i=0; i < array_size; /* increment of i is done within the loop */)
    *(iter++) = 2 * (i++) + 1;
  num* end = ptr + array_size;
  std::cout << scope_indent;
  for(iter = ptr; iter != end; ) std::cout << *(iter++) << ' ';
  std::cout << std::endl;

  rem("Check the ---.... something is wrong isn't it ?");

  return 0;
}

/*
  Have you noticed what is actually wrong ? 

  Only the memory allocated in the stack is released (---). 

  In C/C++, you have to tell the system that you do not need the
  memory of the heap anymore. If you forget it, as we did here, the
  memory allocated is still reserved while the bytes have become
  useless. 

  Here, as we quit, the system stops the process and releases all the
  memories (stack, text, heap, data) that was allocated for it. But if
  such memory is created many times, for example in a loop, and is not
  released, the amount of useless memory grows as the program runs
  (think about a video surveillance program that takes memory at each
  video frame and does not release it when the frame processing is over,
  the amount of memory of the process grows at each frame until
  "memory overflow").

  From a system point of you, it is as if your process actually
  needs the memory, so your process keeps on asking for new memory
  bytes. Such behavior is called ***memory leaks***, it is a bug from
  ***your*** code. Tools like valgrind enable to track such errors.

  Next example is exactly this one, but we add the required
  instructions for releasing the memory taken in the heap (keep in
  mind that in the stack, the release is automatically done when you
  close scopes).

*/
