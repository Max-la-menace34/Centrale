#include <iostream>
#include <string>    // For string manipulations.
#include <algorithm> // for std::copy
#include "num.hpp"

/*

  In this section, we will take advantage of the heap in order to
  represent a data of a certain type, in the case of all data of that
  type have not necessarily the same size in memory. This size cannot
  be determined at compile time, so we need the heap !

  We will trace everything, so we need a quite simple example, that
  will sound unfortunately a bit artificial.

  Let us implement the type "syr", that represents a Syracuse
  serie. Syracuse series are defined by :
  
  U(n+1) = 3*U(n) + 1 // if U(n) is odd
           U(n)/2     // if U(n) is even.

  For example, if U(0) = 11, the serie is :
  11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1, 4, 2, 1, 4,...

  It seems (that is the Collatz conjecture) that every serie ends by
  4,2,1 period. So we will stop the serie when 1 is reached.

  The type syr will store the values. Depending on the start,
  different amount of values need to be stored.

  Let us implement a - operator, that takes two series and returns the
  longest common suffix. The return value is a syr as
  well.

  We would like to write :
  
  syr u {11}; // i.e. 11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1
  syr v {15}; // i.e. 15, 46, 23, 70, 35, 106, 53, 160, 80, 40, 20, 10, 5, 16, 8, 4, 2, 1

  syr k; // default construction
  k = u;
  k = u - v; // 40, 20, 10, 5, 16, 8, 4, 2, 1
  syr l = k; // copy construction
  std::cout << u << std::endl
            << v << std::endl
            << k << std::endl
	    << syr::end() << std::endl;  // syr::end() = 4, 2, 1


  When you need to design some type, start by writing the main you
  would like to have. This is what we did above. So now, we know that
  we will need a default constructor, a constructor taking an int
  (that is U(0)) as an argument, an affectation operator, a
  substraction operator, a printing operator. A static function
  syr::end() will also have to be implemented.

  A syr value will be made of the elements in the serie, and the
  number N of these elements.  We will store the serie values as an
  array of nums, in the heap.

  A piece of memory organized as a syr, i.e. an instance of syr in the
  stack, would therefore be as this :

          STACK            HEAP
  
          +-------+       +-------+
 elements |   ----+-----> |   8   +  elements[0]
          +-------+       +-------+
        N |   4   |       |   4   |  elements[1]
          +-------+       +-------+
                          |   2   |  elements[2]
                          +-------+  
                          |   1   |  elements[3]
                          +-------+  
			  
  sizeof(syr) will thus be the size of an address (the so called
  "elements" attribute) added with the size of an int, the "N"
  attribute. This is defined at compiling time. The variability of the
  elements do not influences sizeof(syr), it happens in the heap.

 */



struct syr {
  /**
   * This static function computes \f$U_{n+1} = f(U_n)\f$
   * @param u the first term, must be strictly positive.
   */
  static int next(int u) {
    if(u % 2 == 0) return u/2;
    else           return 3*u + 1;
  }

  /**
   * @param u the first term.
   * @returns The number of terms when starting at u.
   */
  static int size(int u) {
    int res = 1;
    while(u != 1) {
      u = next(u);
      ++res;
    }
    return res;
  }

  num* elements;   //!< This is the array of elements.
  int  N;          //!< This is the size of the elements array.

  /**
   * Default constructor makes an empty serie.
   */
  syr() : elements(nullptr), N(0) {
    scope("syr()");
  }

  /**
   * This is used for operator-, do not use it in external code.
   */
  syr(int N, num* elements) : elements(elements), N(N) {
    scope(std::string("syr(") + std::to_string(N) + ", num*) internal");
  }
  

  /**
   * Constructor from a seed.
   */
  syr(int seed) : N(size(seed)), elements(nullptr) {
    scope(std::string("syr(") + std::to_string(seed) + ")");
    // We have to allocate the element array. We first initialized it
    // at nullptr, since at attribute initialization time, N may not
    // be known (it depends on the order of initialization).
    this->elements = new num[this->N];
    
    // We use the argument seed as a variable (seed = next(seed)), it
    // is in the stack as a local variable would be.
    num* iter = this->elements;
    for(/* nothing here */; seed !=1 ; seed = next(seed))
      *(iter++) = num("tmp", seed);
    *(iter++) = num("tmp", seed); // We put the last 1 value.
  }

  syr(const syr& other)
    : N(other.N),
      elements(nullptr) { // elements(other.elements) would have been a big mistake...
    /* ... Indeed, if both this->elements and other.elements are
       equal, i.e. they contain identical addresses, both refer to the
       same chunk of memory. If other dies and releases its memory (we
       will see how hereafter), this->elements will still contain an
       address of that released memory. If someone tries to access it,
       or to release it, you will get a segmentation fault in the
       first case or a double free corruption in the second case.

       Here, each syr object must handle its own version of the
       elements. So in this copy constructor, we have to build a new
       one, with identical values... this is what a copy actually is
       after all.
    */
    scope("syr(const syr&)");
    if(other.N > 0) {
      this->elements = new num[other.N];
      num* end = other.elements + other.N;
      
      // We could use :
      //   std::copy(other.elements, end, this->elements);
      // But we rewrite it for the sake of illustration.
      num* output_it = other.elements;
      num* input_it  = this->elements;
      while(output_it != end)
	*(input_it++) = *(output_it++);
    }
  }

  /*
    sizeof(syr) is known at compile time. It is the size of an int
    added by the size of a pointer, as stated before. This is what the
    stack contains when we have syr variables, see the ascii art
    above. So when a syr object is removed from the stack, releasing
    only that syzeof(syr) amount of memory causes a memory leak in the
    heap, since the values, situated at the address in "elements", are
    still reserved for the process. A delete should occur (see
    example-005-002).

    We have stated that constructors are indeed initializers, called
    just after the allocation of the bytes by the system to initialize
    them with proper values. C++ offers, symmetricaly, the access to a
    cleaner function, whose job is to clear the memory before actually
    releasing it. As constructor are not called initializers as they
    should, this function is not called a cleaner but a
    ***destructor***. If constructors allocate some memory ***in the
    heap***, you should have destructors that applies delete on that
    memory, in order to release it when your object dies.

    You can also write things in the destructor, or notifying some
    memory inspector that you are done with the memory. Our num object
    actually print the "---" messages in the destructor of the type
    num (check num.hpp, you have learnt enough to understand how it is
    made).

    As for constructors which are named like the type and han no
    return type, there is a convention for naming the destructor,
    i.e. the cleaning ***method*** (it has an implicite "this"
    argument). The convention tells that no return type has to be
    specified, as for constructors, and that the name is the name of
    the type ("syr" here) preceeded by a ~.
  */
  ~syr() {
    fun_scope;
    delete [] elements;
  }

  /*
    The destructor is called implicitly before any release of the
    memory. Here are several cases:
   
    {
       syr s;
    } // here when the stack is released.

    {
       syr tab[10];
    } // here once for each of the 10 syr instances,  
      // when the stack is released.

    syr* tab = new syr(20);
    syr* tib = new syr[10];
  
    delete    tab; // destructor called once.
    delete [] tib; // destructor called 10 times.
  */ 

  // The assignment operator
  syr& operator=(const syr& other) {
    fun_scope;
    // As for copy, this->elements = other.elements would be an error.

    if(this != &other) {
      // This compares the address of other with "this", in order to
      // check that we actally handle two different chunks of memory.
      
      // this == &other if "s = s;" happens... so we check that the
      // assignment is a "real" one, from one variable to an actual
      // other one. If you do not check this, the following code
      // will lead to a failure in case of a call similar to s = s.

      this->N = other.N;

      // We release the current content. Deleting nullptr is ok (it
      // does nothing) so we do not have to test if elements is
      // nullptr or an actual address.
      delete [] this->elements;
      if(other.N > 0) {
	this->elements = new num[other.N];
	std::copy(other.elements, other.elements + other.N, this->elements);
      }
      else
	this->elements = nullptr;
    }

    return *this;
  }

  syr operator-(const syr& other) const {
    fun_scope;

    if(other.N == 0 || this->N == 0)
      return {}; // By default construction.

    // If we reach this code, both series contain at least one element. 

    // Here, we have to start reading the elements from the end, in
    // parallel, and stop when they differ or when the iteration
    // reaches the first element for one of the two elements (they may
    // not have the same size....). This can be written nicely when
    // pointers are considered as iterators.

    num* start_this  = this->elements + (this->N - 1); // iterator on the last element.
    num* start_other = other.elements + (other.N - 1); // iterator on the last element.
    // These are the elements before the first ones. They do not
    // exist, but they are used as a stop value for iterators.
    num* stop_this  = this->elements - 1; 
    num* stop_other = other.elements - 1;
    num* it_this; 
    num* it_other;
    // We use the for without a body : for(INIT; TEST; INCREMENT);
    // See example example-002-001.
    for(it_this = start_this, it_other = start_other; 
	(it_this != stop_this) && (it_other != stop_other) && (*it_this == *it_other);
	--it_this, --it_other); 
    // We are at one of the stop iterator, or at a position where the
    // series differ (and are the identical until the end). The length
    // of the common part is thus start - it... indeed, the difference
    // between two pointers is not the difference between the
    // addresses, but that difference divided by sizeof(num). In other
    // words, it1 - it2, when they are both iterators on the same
    // array, is the number of elements between the two.
    int result_N         = start_this - it_this;
    num* result_elements = new num[result_N]; // Label #AllocResult
    // we will refer to the line above in further comments.
    std::copy(it_this + 1, start_this + 1, result_elements);
    
    return {result_N, result_elements};
    // We use the syr(int, num*) constructor, so that result_elements
    // is now handled by the newly created object. This object will
    // delete[] this array when it will be released, thanks to the
    // destructor.

    // Tricky isn't it ?
  }

  static syr end() {
    scope("syr::end()");
    return {4}; // This builds 4,2,1
  }

};

std::ostream& operator<<(std::ostream& os, const syr& s) {
  if(s.N == 0)
    os << "<empty>";
  else {
    os << '[';
    num* end  = s.elements + s.N;
    num* iter = s.elements;
    os << *(iter++); // No space in front of the first element.
    while(iter != end)
      os << ' ' << *(iter++);
    os << ']';
  }

  return os;
}


int main(int argc, char* argv[]) {
  fun_scope;
  std::cout << scope_indent << "Enter a test number : " << std::flush;
  int test_nb;
  std::cin >> test_nb;

  if(test_nb == 1) {
    scope("Test #1");
    syr s;
    std::cout << scope_indent << s << std::endl;
    ___;
    syr t = syr::end();
    std::cout << scope_indent << t << std::endl;
    ___;
  }

  
  else if(test_nb == 2) {
    scope("Test #2");
    ___;
    rem("syr s{5}");
    syr s {5};
    std::cout << scope_indent << s << std::endl;
    ___;
  }
  
  
  else if(test_nb == 3) {
    scope("Test #3");
    ___;
    rem("syr s{5");
    syr s {5};
    std::cout << scope_indent << s << std::endl;
    ___;
    rem("syr t = s");
    syr t = s;
    std::cout << scope_indent << t << std::endl;
    ___;
  }
  

  else if(test_nb == 4) {
    scope("Test #4");
    ___;
    rem("syr s{5}");
    syr s {5};
    std::cout << scope_indent << s << std::endl;
    ___;
    rem("syr t{4}");
    syr t {4};
    std::cout << scope_indent << t << std::endl;
    ___;
    rem("t = s");
    t = s;
    std::cout << scope_indent << t << std::endl;
    ___;
  }
  
  

  else if(test_nb == 5) {
    scope("Test #5");
    ___;
    syr s {11};
    std::cout << scope_indent << s << std::endl;
    ___;
    syr t {15};
    std::cout << scope_indent << t << std::endl;
    ___;
    syr u = s - t;
    std::cout << scope_indent << u << std::endl;
    ___;
  }

  else if(test_nb == 6) {
    scope("Test #6");
    ___;
    syr a {8};
    syr b {4};
    syr c {2};
    ___;
    syr d = (a - b) - c;
    std::cout << scope_indent << d << std::endl;
    ___;
  }
    
  
    
  else {
    std::cout << scope_indent << "Bad number entered." << std::endl;
  }
  
  return 0;
}


/* 
   Try the different examples, in order to see how the num contents
   are allocated and released. nums are only used in the arrays here,
   so all the nums you will see are from the heap.

   Let us have a look at test #6.

   The expression (a - b) - c is operator-(operator-(a,b), c),
   i.e. something like f(f(a,b),c). Let us differentiate the two calls
   of f, and consider f1(f2(a,b),c), even if here,
   f1=f2=syr::operator-. 
   
   f1 and f2 calls produce a new syr each. You can see them in the
   trace, since this production involves our internal constructor. The
   allocation is made at the line we labelled #AllocResult in the
   "operator-" method. The total amount of syr objects created is 5 :
   - a, b, c, 
   - the return of f2 (see first internal constructor trace), 
   - the return of f1 (see second internal constructor class).

   The return of f1 is the variable d. The compiler is clever with
   returns, we have seen that already. If a variable is declared and
   initialized as the result of a function call, the return value of
   that function is directly the variable. 

   The return of f2 is built in order to be passed as the first
   argument of the f1 call. Then, once the computation of f1 is done,
   the place for the return of f2 is released, since nobody will use
   it in the future (there is no way to name this chunk of memory in
   the code, for further manipulation). The corresponding memory that
   has been allocated for it is released (you see the release just
   before the printing on the terminal "[2 1]". 

   Allocation and release in the heap may take time, since it invokes
   the heap memory manager (the management of the stack, that is a
   simple push/pop, is much faster). It would have been nice to reuse
   this memory... for example in order to build the result of f1 !

   In other words, if f1 were able to know that its first argument will
   die, we say that its first argument is ***an expiring value***, it
   could use the memory at addres arg1.elements for result.elements,
   since in our case it has enough space, without needing a
   supplementary allocation. This cannot be done for the call f2,
   since a and b may be reused afterwards and need to have their own
   memory untouched.

   Here, f1 and f2 is the same function, and thus the same piece of
   code. It would be nice to be infomed about the expireness of the
   arguments, in order to optimize expensive memory allocation. This
   is not feasible with the mechanisms we have seen so far, but
   indeed, this is the kind of things that can be determined at
   compiling time. Since C++11, a new kind of constructor has been
   introduced, called the ***move contructor***, dedidaced to such
   memory optimizations with the heap attributes. This is out of the
   scope of this tutorial, but the idea for now is that you identify
   the problem, and be sensitive to the power of optimization provided
   by C++11 features.
*/
