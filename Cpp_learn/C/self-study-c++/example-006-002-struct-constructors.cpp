#include <iostream>
#include "num.hpp"

/* 
   This is a re-writing of the previous example with an object
   oriented flavor. The previous example oand the current one will
   produce quite the same executable, once compiled. So everything
   intriduced here is only a matter of syntax. Keep opened the code of
   the previous exampke while you read this one, in order to compare
   the two formulations of the same execution, i.e the one you had
   previously and the new one we intriduce here. 
*/

/*
  This example introduces the concept of constructor, and some common
  operators overload. We do the same as in the previous example, but
  with the help of the object oriented features of C++. Nothing is
  really new, in term of memory management, what we introduce is the
  syntax for simplifying the job of the programmer of the grid_point
  type.

 */

/* Do not get confused in the following, since now, different things
   are called "grid_point". 

   We still have the name of the type, since we declare "struct
   grid_point {....};", allowing us to declare variables as
   "grid_point p;".

   Other things named "grid_point" as well are the initialization
   functions introduced here. In object oriented language, the
   initialization of a value of type T is a function whose name is T
   as well ! The initializer is assorted to the value type, so it is
   convenient to give it the same name as the type.
*/


struct grid_point { // We define the grid_point type, as previously.

  // These are the attributes.
  
  num x;
  num y;

  // This is the previous
  //    void grid_point_init(grid_point* that, num& x, num& y) {...} 
  // initializing function. It is called "grid_point" now, rather that
  // "grid_point_init" as previously.
  //
  // It is define inside the "struct grid_point {...};"
  // declaration. This function is called a "method" oof the structure
  // "grid_point" in this case.
  grid_point(num& x, num& y) {
    // It seems that the "that" argument of grid_point_init is
    // missing. Indeed, there is one "that" argument, but as this is
    // very common in object oriented programming to have it, there is
    // no need to (and you shouldn't) write it. There is a keyword for
    // that "that" argument, which is "this".

    // So even if this function declaration seems to be introducing x
    // and y argument only, it introduces a "this" argument, as if
    // we had declared:
    //     grid_point(grid_point* this, num& x, num& y) {...}
    // ... so we can use it, since it is there !
    // Methods are just functions, with an implicit argument named "this",
    // that plays the role of the "that" pointer we had in the prevous
    // example. The purpose is only to have shorter writings, nothing will
    // really change in the executable that is compiled.


    // So as there is a "this" argument, we can use it even if it is
    // not appearing the the function arguments.
    this->x = x;
    this->y = y;

    // Something else is missing here, the "void" return type. Indeed,
    // it is not necessary to write it, so it is omitted. You are
    // allowed to omit "void" for initializing functions only. C++
    // recognize them since their name is the same name as the type,
    // as previously stated.
  }


  /*
    To sum up, we have written our initializer function within the
    "struct grid_point {...};" part of the type definition. It has no
    return type, and its name is grid_point, i.e. the name of the
    type. Such initializing function should be called an initializer,
    since it operates just after memory allocation to set the values
    of this memory, but the official name for this is
    ***constructor***.  We will name initializer as constructors from
    now, but keep in mind that the function does not allocate the
    memory, this has been done by the memory manager just before the
    call of the constructor.
  */


  // We have defined previously another initializer :
  //   void grid_point_copy(grid_point* that, grid_point&  other_point) {...}
  // Let us write it here as we did for grid_point_init. We thus write
  // another constructor of the class.

  grid_point(const grid_point& other_point) {
    this->x = other_point.x;
    this->y = other_point.y;
  }

  
  /*
    Two points need to be noticed. 

    First, the keyword const. It means
    that the reference other_point should not be used to modify the
    argument that is passed. The compiler checks it. We could have
    added this "const" keyword in the definition of
    grid_point_copy. We could also have used const for the references
    of the two nums in the first constructor (and in grid_point_init
    previously). We will add it in next example :
       grid_point(const num& x, const num& y) 
    
    When we will do this, writing "x = something" in the code of
    "grid_point(const num& x, const num& y)" causes the compiler to
    stop with an error message, which helps the developper to remind
    that the reference x should not be used to modify the argument
    passed by reference. There is no memory mechanism added when you
    write const, you just add compiler checkings.

    Second, the initializer has the same name as the other. This is
    indeed not an issue (it is ok in C++, but in C all functions must
    have different names). The trick is that the compiler can make the
    difference between functions having the same name from the types
    of the arguments passed when it calls them. So this is ok when the
    functions do not have the same kind and/or the same number of
    arguments. So the functions grid_point(num& x, num& y)
    grid_point(const grid_point& other_point) are not the same, in
    spite of their common name. When you call some "grid_point"
    functions, the compiler knows which one it is by looking at the
    type of the arguments. This is called ***polymorphism***.

   */
  
};




// Let us rewrite the printing function as well, such as it is
// complient with natural printing in C++, using the << operator. This
// is a case of operator overload that we do not detail, so take the
// following as a receipe for printing your types, for the
// moment. "std::ostream" is a type (as int, num, grid_point...) and
// "operator<<" the name of the function. Note the use of const for the
// point that we only read for printing. We take a reference in order
// to avoid a useless copy of the grid_point we want to print.

// Nota : we are out of the "struct grid_point {...};" scope, so there
// is no hidden "this" argument in the functions we
// declare... i.e. they are not methods but usual functions.

std::ostream& operator<<(std::ostream& os, const grid_point& the_one_to_print) {
  os << '(' << the_one_to_print.x << ", " << the_one_to_print.y << ')'; // We have removed std::endl.
  return os;
}

// Ok, let us play with these brand new tools in the main. That main
// is similar to the one of the previous example.


int main(int argc, char* argv[]) {
  fun_scope;
  
  rem("We allocate then two a, b nums");
  
  num a {"a", 14};
  num b {"b",  7};
  
  ___;

  rem("we ***allocate and*** initialize 'p' with 'a' for 'x', 'b' for 'y'");

  grid_point p {a,  b};
  // We have here the 3 (not 2 !) arguments of the first constructor :
  // - p : its address &p is the argument this.
  // - a : x, passed by reference, is a reference on a.
  // - b : y, passed by reference, is a reference on b.
  // So the line means : "allocate p in the stack, and then call
  // grid_point(&p, a, b)". We do not call contructors explicitly. Here
  // the first of them is called when we declare the variable p, providing
  // {a, b} so the compiler knows which of the two initializers
  // named "grid_point" it has to use.
  
  ___;

  rem("Now we print p");

  std::cout << scope_indent << p << std::endl; // We put std::endl here, not in the printing function.
  // This is indeed
  //     ((std::cout << p) << std::endl);
  // and the first "std::cout << p" is
  // operator<<(std::cout, p)... this matches our operator<< function definition,
  // - std::cout : this is the argument os passed by reference, of type std::ostream
  // - p         : this is the argument "the_one_to_print", passed by reference.

  ___;
  
  rem("we ***allocate and*** initialize q so that is has the same attributes than p");

  grid_point q {p};
  // The following is legal as well :
  //   grid_point q = p;
  // We call the second constructor : grid_point(&q, p), with
  // - &q : the "this" argument
  // - p  : the "other_point" argument, passed by reference and declared as const.
  
  ___;

  rem("Now we print q");

  std::cout << scope_indent << q << std::endl;

  ___;
  

  rem("Cleaning and exit");
  return 0;
  
}

/*

  The code is nicer, initializers related to grid_point are defined
  within the "struct grid_point {...}" part of the code, as the
  attributes are. Initializations are performed just after variable
  declarations, with a light syntax rather than an explicit call of
  the initialization function after the variable declaration. Cool.

  But there is still one issue. When you look to the trace, you will
  see that attributes are initialzed by default when the memory is
  allocated, and then initialized as you like, with your constructor,
  by an assignment. You see "+++ ..." traces followed by "=== ..."
  traces each time an attribute is allocated in the memory. Indeed,
  there is a place where to write attribute initialization so that the
  compiler applies your code when it allocates the memory. It is
  before the braces...

  grid_point (num& x, num& y) 
  <here is the place for attribute initialization !>
  {
      .....
  }

  Let us do it in next example.

*/
