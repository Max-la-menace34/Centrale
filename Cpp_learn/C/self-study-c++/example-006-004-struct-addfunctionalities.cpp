#include <iostream>
#include <iomanip> // for std::boolapha
#include "num.hpp"


/*

  Let us implement other functions for grid_point.

 */


// This is the previous code, without comments.
struct grid_point {
  num x;
  num y;
  
  grid_point(const num& x, const num& y)    : x(x), y(y)                         {}
  grid_point(const grid_point& other_point) : x(other_point.x), y(other_point.y) {}
  grid_point()                              : x("0.x", 0), y("0.y", 0)           {}
};

std::ostream& operator<<(std::ostream& os, const grid_point& the_one_to_print) {
  os << '(' << the_one_to_print.x << ", " << the_one_to_print.y << ')';
  return os;
}


// Ok let us define new functions for handling points. We choose
// arbitrarily to call the first grid_point argument
// "this_point".... it will become a "this" in next example. But for
// now, consider it as a name, that could have been any other name.


void grid_point_raz(grid_point& this_point) {
  this_point.x = 0;  // nums can be assigned with integers....
  this_point.y = 0;
}

grid_point grid_point_add(const grid_point& this_point, const grid_point& other_point) {
  fun_scope;
  grid_point res {this_point.x + other_point.x, this_point.y + other_point.y};
  return res;
}

grid_point grid_point_sub(const grid_point& this_point, const grid_point& other_point) {
  fun_scope;
  return {this_point.x - other_point.x, this_point.y - other_point.y};
  // This is the same as previously, res was not really necessary.
}

num grid_point_dot_product(const grid_point& this_point, const grid_point& other_point) {
  fun_scope;
  return this_point.x * other_point.x + this_point.y * other_point.y;
  // No braces here, we compute a num from the attributes.
}

num grid_point_distance2(const grid_point& this_point, const grid_point& other_point) {
  fun_scope;
  grid_point tmp = grid_point_sub(this_point, other_point);
  return grid_point_dot_product(tmp, tmp);
}

bool grid_point_is_equal(const grid_point& this_point, const grid_point& other_point) {
  fun_scope;
  return this_point.x == other_point.x && this_point.y == other_point.y;
}

bool grid_point_is_different(const grid_point& this_point, const grid_point& other_point) {
  fun_scope;
  return this_point.x != other_point.x || this_point.y != other_point.y;
}

grid_point grid_point_zero() {
  // You can call a constructor directly in a piece of code,
  // explicitely. For example, for num, you can write "num("a", 15)"
  // in the code. It creates a new variable initialized with the
  // constructor... but it has no name that you can use. It is
  // convenient for passing arguments. We will use this for building
  // up the point zero from scratch.
  return {num("0.x", 0), num("0.y", 0)};

  // This would have worked as well, calling directly the constructor
  // grid_point, as well as calling directly, twice, the constructor
  // num for its arguments.
  //   return grid_point(num("0.x", 0), num("0.y", 0));
}

grid_point grid_point_from_ints(int x, int y) {
  return {num("int.x", x), num("int.y", y)};
}


// Ok, let us use it.

int main(int argc, char* argv[]) {
  grid_point a {num("a.x", 10), num("a.y", 5)};
  grid_point b {-a.y, a.x};

  ___;

  rem("Testing distance");
  
  num d2 = grid_point_distance2(a, b);
  std::cout << scope_indent << "d2(" << a << ", " << b << ") = " << d2 << std::endl;
  
  ___;
  
  rem("Testing orthogonality");

  // std::boolapha prints the booleans as "true/false" rather than "0/1".
  bool ortho = (grid_point_dot_product(a, b) == 0); // nums compare to integers.
  std::cout << scope_indent << "Are " << a << " and " << b << " orthogonal ? "
	    << std::boolalpha << ortho
	    << std::endl;

  ___;
	       
  rem("Testing from_ints");
  
  bool equal = grid_point_is_equal(a, grid_point_from_ints(10, 5));
  std::cout << scope_indent << "This is supposed to be true, is it ? " << std::boolalpha
	    << equal
	    << std::endl;
  ___;

  rem("Testing raz");
  grid_point_raz(a);
  equal = grid_point_is_equal(a, grid_point_zero());
  std::cout << scope_indent << "This is supposed to be true, is it ? " << std::boolalpha
	    << equal
	    << std::endl;

  return 0;
}
