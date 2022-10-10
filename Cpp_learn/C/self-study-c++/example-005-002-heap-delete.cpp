#include <iostream>
#include "num.hpp"

/*

  Release the memory taken in the heap with the "delete" operator.

  Except for the delete, the code is quite identical to the previous
  example. So we remove many comments.

 */

int main(int argc, char* argv[]) {
  fun_scope;

  num i;            
  num j {"j", 26};   
  num* ptr = &i; 
  std::cout << scope_indent << "ptr = " << ptr << ", *ptr = " << *ptr << std::endl;
  ptr = &j; 
  std::cout << scope_indent << "ptr = " << ptr << ", *ptr = " << *ptr << std::endl;

  ___;

  num* ptr1 = new num; // Now, we use one variable per pointer, since
		       // we will need them for incoming releases.
  rem("ptr1 = new num;");
  std::cout << scope_indent << "ptr1 = " << ptr1 << ", *ptr1 = " << *ptr1 << std::endl;
  ___;

  num* ptr2 = new num("k", 12);
  rem("ptr2 = new num(\"k\", 12);");
  std::cout << scope_indent << "ptr2 = " << ptr2 << ", *ptr2 = " << *ptr2 << std::endl;
 
  ___;


  rem("ptr3 = new num[array_size];");
  int array_size;
  std::cout << scope_indent << "Enter the size of the array : " << std::flush;
  std::cin >> array_size;
  num* ptr3 = new num[array_size];
  rem("The array is allocated, and initialized by default.");
  num* iter = ptr3;
  for(int i=0; i < array_size; /* increment of i is done within the loop */)
    *(iter++) = 2 * (i++) + 1;
  num* end = ptr3 + array_size;
  std::cout << scope_indent;
  for(iter = ptr3; iter != end; ) std::cout << *(iter++) << ' ';
  std::cout << std::endl;

  ___;

  rem("let us clean the heap now, since");
  rem("we do not need the memory anymore.");
  ___;
  
  rem("cleaning ptr1 and ptr2...");

  // First, let us clean ptr1 and ptr2.
  delete ptr1;
  delete ptr2;
  
  // The memory where the two nums lived is now free to be reused by
  // other heap allocations, or even by other processes. Nevertheless,
  // the pointer variable ptr1 and ptr2 still contain the integer they
  // had before delete. If you write *ptr1, your process will try to
  // read at the address still stored in ptr1, and your process may
  // receive a "segmentation fault" signal from the system and
  // die... I say "may receive", because there is a much worse
  // case. It the memory adressed by ptr1 is still yours (i.e. the
  // system still consider it is allocated to the current running
  // process), then no signal is sent, and everything runs with weird
  // values for *ptr1. Bugs will have consequences later in this case,
  // which make them very difficult to fix.

  // If you type now "delete ptr1", the process will ask the memory
  // manager to release the address in ptr1 while this has already
  // been done. This bug is called a "double free corruption"
  // ('releasing', which is also 'deleting' is called 'freeing' in the
  // core memory management). This is a bug as well. You may have an
  // error such as "segmentation fault" that do not mention 'double
  // free' explicitly. You need to invoke specific memory debugging
  // tools, as valgrind, to be notified with a clear "double free
  // corruption" for this. Using such tools is another topic, this is
  // not addressed here.

  // More generally, if a pointer variable contains an integer that
  // corresponds to nothing that have been allocated ***in the heap***
  // by new (a silly uninitialized value, an address of memory in the
  // stack, ...), deleting it will cause a crash of your process.

  // You can put a specific value in pointer variables, telling that
  // they do not contain an address of something.
  ptr1 = nullptr;
  ptr2 = nullptr;

  delete ptr1; // This does nothing, and no crash, since the content is now nullptr.
  delete ptr2; // This does nothing, and no crash, since the content is now nullptr.

  // Setting with nullptr is not mandatory if you do not reuse the
  // address stored in a pointer.

  ___;

  // We still have to release the array ptr3. It is a pointer as well,
  // but the memory manager cannot guess that ptr3 actually refers to a
  // full bunch of nums, rather than only one. You have to specify it
  // by adding "[]" to your delete instruction.

  rem("cleaning ptr3, that is an array...");
  delete [] ptr3;

  // Be very carefull, "delete [] p" if p is in the heap, but not
  // allocated with "new int[...];" is a bug, and "delete p" when p is
  // allocated with "new int[...];" is a bug as well. You have to be
  // very cautious when you play with the heap, bugs hide there.
  // These are bugs because they will not do what you may expect but
  // they will actually do something and the compiler will accept
  // them.
  
  // The following code :    
  //     int* toto = new int;
  //     delete [] toto;
  // can be compiled and executed and it is actually freeing some
  // memory but not the one you expect.

  // The following code :
  //     int *toto = new int[10];
  //     delete toto;
  // can be compiled and executed and it is actually freeing some
  // memory but only the memory used for storing the very first
  // element of the array and there are still a bunch of memory that
  // remains considered as reserved by the memory manager.
  
  ___;

  rem("That's all folks ! The stack will be release and we quit.");

  return 0;
}


/*

  We have written num that behaves like an int, but is smarter (and
  thus less efficient). The same thing exists for pointers in the STL
  (they are smarter and almost as efficient as native pointers). These
  are called "smart pointers". Smart pointers behave as num* do, from
  a syntactical point of view, but they handle many issues concerning
  delete and many other things. Using smart pointers is out of the
  scope of this tutorial. You will appreciate them when you will be
  familiar with pointers.

 */
