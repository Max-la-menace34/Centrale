#include <iostream>
#include "num.hpp"


/*

  Improvement of the previous example, in order to avoid a default
  initialization of the attribute before setting them to the value
  that we want in our constructors.

 */


struct grid_point {

  num x;
  num y;


  // Initialization is between ":" and "{". It has the form
  // <name_of_an_attribut>(....). Be carefull, the attributes have to
  // be initialized in the same order as their declaration (x, and
  // then y here).
  grid_point(const num& x, const num& y)
    : x(x),
      y(y) {
    // Display could be done here, as we did for num so that they
    // trace "+++ ...". But for this example, nothing is
    // necessary. The two braces are still mandatory, we write {} in the
    // following.
  }
  
  grid_point(const grid_point& other_point)
    : x(other_point.x),
      y(other_point.y) {}
  // This is called the ***copy constructor***.

  // We can add another constructor, with no arguments.
  grid_point()
    : x("0.x", 0),    // attribute x is initialized as if we write "num x {"0.x", 0};"
      y("0.y", 0) {}  // The same for attribute y.
  // This is called the ***default*** constructor. It has no argument...
  // ... other that "this", that is still there of course. This constructor is
  // called when you write
  //   grid_point p;
  // and it is also called 10 times if you write
  //   grid_point tab[10];
  
  
};


std::ostream& operator<<(std::ostream& os, const grid_point& the_one_to_print) {
  os << '(' << the_one_to_print.x << ", " << the_one_to_print.y << ')';
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

  rem("we ***allocate and*** initialize p with a for x, b for y");

  grid_point p {a,  b};
  
  ___;

  rem("Now we print p");

  std::cout << scope_indent << p << std::endl; 

  ___;
  
  rem("we ***allocate and*** initialize it so that is has the same attributes than p");

  grid_point q {p};
  
  ___;

  rem("Now we print q");

  std::cout << scope_indent << q << std::endl; 

  ___;
  
  rem("We initialize a grid_point 'by default'");
  
  grid_point k; // This uses the grid_point() constructor.
  
  ___;
  

  rem("Cleaning and exit");
  return 0;
  
}


// Read carefully the traces, there are no more unnamed "#1234"-like
// variables. The primes (', '') reminds you where the copy came from.
