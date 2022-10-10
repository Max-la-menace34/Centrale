#include <iostream>
#include "num.hpp"

/*
  
  In this section, we introduce the creation of new types from already
  existing ones. This is what have been done in num.hpp so that num,
  which is not a standard C++ type, is available for this tutorial.

  Let us define a grid point, which is a pair (x,y) of integer
  coordinates. This new type consists of the concatenation of two
  nums, x and y. Let us write this.

 */


struct grid_point {
  num x;
  num y;
};
// Do not forget ";" after the closing brace, you would get syntax
// errors on the next line of code (void grid_point_print...), which
// is confusing since the error would be indeed here.


// A new type grid_point is available ! It can be used as int, num,
// .... Let us write a function that prints a grid_point. It is nice
// (but not mandatory) to name such functions with a name like
// grid_point_something.

void grid_point_print(grid_point that_point) {
  fun_scope;
  rem("je suis la ");
  std::cout << scope_indent << '('
	    << that_point.x // that_point.x is the first of the two nums agglomerated in a gridpoint.
	    << ", "
	    << that_point.y // that_point.y is the second.
	    << ')' << std::endl;

  // x and y, used in expressions like that_point.x and that_point.y
  // are called "members" or rather "attributes" of the grid_point
  // type. The "struct grid_point {...};" statement thus consists in
  // defining the attributes of a custom type.
}


// Let us build a function that initializes a grid_point. As we did
// for raz, we need to pass the grid_point by reference, so that it
// can be changed by the function (a function that takes a copy of a
// grid_point and initializes that copy is useless). We have seen two
// equivalent ways to pass arguments by reference, with a pointer or
// with a reference, the latter being a syntactical convenience for
// writing the first. Let us use the native pointer-version of it.

void grid_point_init(grid_point* that,
		     num x, num y) {
  fun_scope;
  // "that" is an address of the grid_point we want to initialize, *that
  // is that grid_point. So (*that).x and (*that).y are the two
  // attributes of that grid_point. Let us initialize them with the
  // arguments x and y.

  (*that).x = x;
  that->y   = y; // This is a syntactic sugar for (*that).y... use it !
}

// We can also init a point such as it has the value of another one.
void grid_point_copy(grid_point* that,
		     grid_point  other_point) {
  fun_scope;
  that->x = other_point.x;
  that->y = other_point.y;
}


// Ok, let us use all this in a piece of code, and see what is
// actually allocated and released. We only use the stack here.

int main(int argc, char* argv[]) {
  fun_scope;

  rem("First, we allocate a grid_point p in the stack");

  // grid_point is a type that we have added to the compiler
  // knowledge. As a type, it behaves as int, num, and others. So you
  // can declare a variable of this type
  grid_point p;
  // This is similar as declaring "int i;" when you want an int
  // variable.
  
  ___;
  
  rem("We allocate then two nums, a and b");
  
  num a {"a", 14};
  num b {"b",  7};
  
  ___;

  rem("we initialize p with a for x, b for y");

  grid_point_init(&p, // the "that" argument...
		  a,  // ...the "x" argument...
		  b); // ...the "y" argument.
  
  ___;

  rem("Now we print p");

  grid_point_print(p);

  ___;

  
  rem("We allocate another grid_point q in the stack");
  
  grid_point q;
  
  ___;
  
  rem("...and we initialize it so that is has the same attributes than p");

  grid_point_copy(&q, p);
  
  ___;

  rem("Now we print q");

  grid_point_print(q);

  ___;
  

  rem("Cleaning and exit");
  return 0;
  
}


/*

At runtime, you see that some nums have been copied (that are named
with as a', b', #1234'). This is a waste of time here, since we pass
the concerned arguments only for reading. Change the code so that all
these useless copies are not created anymore, and test.

Hint : you only need to add four '&' symbols in the code.

Once this is done, you may notice that we first allocate a grid_point,
having its attributes allocated by default, and then we immediatly
initialize them. In the next example, we will see how to perform
allocation and initialization all at once.

 */
