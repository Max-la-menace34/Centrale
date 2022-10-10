#include <iostream>
#include <iomanip> // for std::boolapha
#include "num.hpp"


/*
  We add more refinments to the previous example, thanks to operator
  overloading.

  Indeed, we write things as
     a.dot_product(b)
  whereas it could be nicer to write
     a * b
  for this method call.

  Indeed, in C++, "a*b" can be understood as "operator*(a,b)" or
  "a.operator*(b)", considering the string "operator*" as a usual
  function name. 

  The interpretation "operator*(a,b)" is what we have done for the <<
  operator, when we defined the strange operator<< function for
  printing. 

  Here, we use the second interpretation "a*b",
  i.e. "a.operator*(b)". It means that "operator*" is the name of a
  method in the struct defining the type of a.

  So we just have to replace some method names in the previous example
  as follows :
      add          --> operator+
      sub          --> operator-
      dot_product  --> operator*
      is_equal     --> operator==
      is_different --> operator!=

  We will also add a new method operator= that allows to handle the
  assignment. 
  
 */

struct grid_point {
  num x;
  num y;
  
  grid_point(const num& x, const num& y)    : x(x), y(y)                         {}
  grid_point(const grid_point& other_point) : x(other_point.x), y(other_point.y) {}
  grid_point()                              : x("0.x", 0), y("0.y", 0)           {}

  // This has to have this form for complience with native type
  // accepted uses of assignment, as "a = b = c;" sequence of
  // assignments. The fact that it returns a reference type is out of
  // the scope of that tutorial.
  grid_point& operator=(const grid_point& other_point) {
    x = other_point.x;
    y = other_point.y;
    return *this; // Take this as a receipe for now.
  }

  void raz() {
    this->x = 0;
    this->y = 0;
  }

  grid_point operator+(const grid_point& other_point) const {
    fun_scope;
    grid_point res {this->x + other_point.x, this->y + other_point.y};
    return res;
  }
  
  grid_point operator-(const grid_point& other_point) const {
    fun_scope;
    return {this->x - other_point.x, this->y - other_point.y};
  }

  num operator*(const grid_point& other_point) const {
    fun_scope;
    return this->x * other_point.x + this->y * other_point.y;
  }

  // The use of opertors makes this code smarter that the code of
  // distance2 in the previous example.
  num distance2(const grid_point& other_point) const {
    fun_scope;
    grid_point tmp = *this - other_point; 
    return tmp * tmp;
  }

  bool operator==(const grid_point& other_point) const {
    fun_scope;
    return this->x == other_point.x && this->y == other_point.y;
  }

  bool operator!=(const grid_point& other_point) const {
    fun_scope;
    return this->x != other_point.x || this->y != other_point.y;
  }

  static grid_point zero() {
    return {num("0.x", 0), num("0.y", 0)};               // Indeed, we do not need "this".
  }

  static grid_point from_ints(int x, int y) {
    return {num("from_int.x", x), num("from_int.y", y)}; // Indeed, we do not need "this".
  }
  
};

std::ostream& operator<<(std::ostream& os, const grid_point& the_one_to_print) {
  os << '(' << the_one_to_print.x << ", " << the_one_to_print.y << ')';
  return os;
}

// Now, the code gets much smarter than the main of the previous
// example, since dot products and comparisons are quite natural.
int main(int argc, char* argv[]) {
  grid_point a {num("a.x", 10), num("a.y", 5)};
  grid_point b {-a.y, a.x};

  ___;

  rem("Testing distance");
  
  num d2 = a.distance2(b);
  std::cout << scope_indent << "d2(" << a << ", " << b << ") = " << d2 << std::endl;
  
  ___;
  
  rem("Testing orthogonality");

  // std::boolapha prints the booleans as "true/false" rather than "0/1".
  bool ortho = (a * b == 0); // nums compare to integers.
  std::cout << scope_indent << "Are " << a << " and " << b << " orthogonal ? "
	    << std::boolalpha << ortho
	    << std::endl;

  ___;
	       
  rem("Testing from_ints");

  bool equal = (a == grid_point::from_ints(10, 5));
  std::cout << scope_indent << "This is supposed to be true, is it ? " << std::boolalpha
	    << equal
	    << std::endl;
  ___;

  rem("Testing raz");
  a.raz();
  equal =  (a == grid_point::zero());
  std::cout << scope_indent << "This is supposed to be true, is it ? " << std::boolalpha
	    << equal
	    << std::endl;

  return 0;
}

