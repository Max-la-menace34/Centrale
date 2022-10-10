#include <iostream>
#include <algorithm> // Provides std::copy used at the end.
#include "num.hpp"

/*
  
  This one is more difficult. Read and experiment carefully. We
  address here smarter manipulations on pointers.
 
 */

#define SIZE 10

int main(int argc, char* argv[]) {
  fun_scope;

  // Let us get and initialize an array in the stack.

  num tab[SIZE]; // We add 10 nums over the stack.
  
  for(int i = 0; i < SIZE; ++i)
    tab[i] = 2*i + 1;

  rem("Tab initialization done, such as tab[i] = 2*i + 1.");
  ___;

  rem("tab is a pointer...");
  std::cout << scope_indent << tab << std::endl;

  // The name tab is not as other names that we used so far. The value
  // of the symbol "tab" is the address of the first num we have
  // stacked. The first value in the array is tab[0], so tab has the
  // same value as &(tab[0]).

  // Let us put tab (which is an address) in a variable over the
  // stack. This variable is a num*.
  rem("num* ptr = tab;");
  num* ptr = tab;
  std::cout << scope_indent << "sizeof(num) = " << sizeof(num) << std::endl
	    << scope_indent << "ptr         = " << ptr         << std::endl
	    << scope_indent << "tab         = " << tab         << std::endl
	    << scope_indent << "*ptr        = " << *ptr        << std::endl
	    << scope_indent << "tab[0]      = " << tab[0]      << std::endl
	    << scope_indent << "&(tab[0])   = " << &(tab[0])   << std::endl;

  ___;

  // Let us print the address of the 10 elements of the array tab.
  rem("for all i, print &(tab[i]).");
  for(int i = 0; i < SIZE; ++i)
    std::cout << scope_indent << "&(tab[" << i << "]) = " << &(tab[i]) << std::endl;


  // Observe the traces. You may notice that the addresses value goes
  // by increment of sizeof(num) bytes (be carefull, addresses are
  // printed in hexadecimal) so an offset of 0x28 is actually an offset
  // of 2*16+8 = 40 bytes

  // It means that arrays are ***consecutive*** blocks of memory put
  // over the stack, as SIZE successive variable declarations would
  // have been.

  ___;

  rem("Pointer increment");
  num* a = tab; // num* a = ptr; would have been the same.
  num* b = a + 1;
  num* c = a + 2;
  num* d = a + 3;
  std::cout << scope_indent << "a = " << a << ", *a = " << *a << std::endl
	    << scope_indent << "b = " << b << ", *b = " << *b << std::endl
	    << scope_indent << "c = " << c << ", *c = " << *c << std::endl
	    << scope_indent << "d = " << d << ", *d = " << *d << std::endl;

  // Adresses are integers. If a is address 3214, a+1 should be
  // 3215. As you may notice from the previous printing, a+1 is
  // a+sizeof(num), and a+i is a+i*sizeof(num). This is very
  // convenient, since a, b, c and d are pointers to the array
  // elements thank to such weird increment policy.

  // This is why pointer variables have a type. C++ could have
  // implement a type "address" for all the addresses (this exists...)
  // instead of int*, num*, ... which are all addresses. But with
  // typing, when the compiler has to compile a+i where i is an
  // integer and a is a sometype*, it computes a+i as
  // a+i*sizeof(sometype).

  // a - i works the same, a++ and a-- also shift with a
  // sizeof(sometype) step.


  // Indeed, tab[i] is *(tab + i). You can write both ones in C++,
  // they are equivalent. The computation actually done on address
  // values is "tab + i * sizeof(num)", so a multiplication and an
  // addition are performed by the processor when you write tab[i].

  ___;

  // Let us multiply the elements of the array by two, thanks to a
  // classical but inefficient loop.
  rem("classical but inefficient loop.");
  for(int i = 0; i < SIZE; ++i) // ++i is one addition per iteration.
    tab[i] *= 2;                // tab[i] needs to compute tab + i *
				// sizeof(num), i.e an addition and a
				// multiplication per iteration.

  ___;

  // Let us divide the elements of the array by two, thanks to a less
  // classical but efficient loop (indeed, this is very classical for
  // good C/C++ programmers).
  rem("The loop of the geeks #1");
  num* begin = tab;        // A pointer to the first element
  num* end   = tab + SIZE; // A pointer to the position just after the
			   // last element.
  
  // ++iter means iter <- iter + sizeof(num).... only an addition per
  // step is involved.
  for(num* iter = begin; iter != end; ++iter) 
    *iter /= 2;

  ___;

  // Let us slightly rewrite this, using the full power of ++
  // operator.. we multiply by 2 here.
  rem("The loop of the geeks #2");
  num* iter = tab;
  // end is available from above code.
  while(iter != end)
    *(iter++) *= 2; // We write the memory chunk *iter (i.e. the chunk
                    // at address iter) and, after, we increment iter
                    // so that is is the address of the next chunk (we
                    // say that it "points to" the next chunk).

  // In the two previous sections, the pointer iter behaves as an
  // ***iterator***. Iterating on collections will have this flavor in
  // the standard template library, even if sometimes the iter object
  // you will get are not pointers. Or in other words, pointers behave
  // as high level iterators on collections while they are efficient
  // basic address computation.

  ___;

  // Let us copy one array into another. For that purpose, let us
  // create another array "tib".
  rem("Allocation and initialization of tib.");
  num tib[SIZE];
  num* it_tib  = tib;
  num* end_tib = tib + SIZE;
  int i = 0;
  while(it_tib != end_tib)
    *(it_tib++) = i++; // i.e. tib[i] = i; skip to next element and increment i.


  rem("Now, we copy tib to tab");

  // tab = tib is not allowed. Indeed, tab (and so is tib) are the value
  // of an address, but not a variable whose content can be changed.

  // num* a = tab; num* b = tib; a = b; This does work as you may
  // expect. It does not copy the content of the array b into the
  // content of the array a. Indeed, now a and b are actual variables
  // whose values are tab and tib addresses. So now, a=b is licit. But
  // is only writes the integer stored in b in the integer container
  // a. This is not copying the 10 elements of tib into the 10
  // variables hosted by tab. We have thus to loop in order to actually
  // copy the elements.

  num* it_tab = tab;
  it_tib = tib;
  // end_tib is already available from previous code.
  while(it_tib != end_tib)
    *(it_tab++) = *(it_tib++);

  // Here it_tab behaves as an ***output iterator*** (it writes data in
  // the collection) and it_tib behaves as an ***input iterator***
  // (it reads values from the collection). These two kind of
  // iterators are concepts used massively in the Standard Template
  // Library.

  ___;

  // The STL provides an algorithm that implements the previous
  // loop. As most algorithms provided by stl, it works on
  // iterators. The pointers that we have here can play that role.

  // Let us redo the same copy with the STL std::copy algorithm.

  rem("std::copy");
  std::copy(tib, tib + SIZE, tab);
  // means copy from addresses tib to tib + size (excluded), and
  // write this from address tab. tib is the input iterator and
  // tab the output iterator, as previously.

  rem("This was really scary... but nice, isn't it ?");
  
  return 0;
}
