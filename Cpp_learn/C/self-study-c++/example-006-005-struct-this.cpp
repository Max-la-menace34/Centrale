#include <iostream>
#include <iomanip> // for std::boolapha
#include "num.hpp"


/*

  The previous example uses many functions like
      ... grid_point_name(const grid_point& this_point, ...)

  We will put those functions in the "struct grid_point {...}" section
  of code, as we did for contructors. Here again, the first argument
  will be turned into a "this" argument, this will be omitted. The
  implicit argument "this" is a pointer, not a reference, so

    ... grid_point_name(const grid_point& this_point, ...) {
        this_point.x ....
    }

  will become 

    ... grid_point_name(...) {
        this->x ....
    }

  As we will define the function in the "struct grid_point {...}"
  section of code, prefixing the function name by "grid_point_" is
  redundant. So we will finally write :

    ... name(...) {
        this->x ....
    }

  But the compiler considers this function having the name
  "grid_point::name". Everything that is defined in the "struct
  grid_point {...}" section of the code has implicitly its name
  prefixed with "grid_point::". Such prefix mechanism is called a
  ***namespace***. The "std::" is a namespace, defined for standard
  things from STL.

  The functions that we add in the "struct grid_point {...}" section
  of the code are another example of ***methods*** of the grid_point
  class (we already know constructors).

 */

struct grid_point {
  num x;
  num y;
  
  grid_point(const num& x, const num& y)    : x(x), y(y)                         {}
  grid_point(const grid_point& other_point) : x(other_point.x), y(other_point.y) {}
  grid_point()                              : x("0.x", 0), y("0.y", 0)           {}

  void raz() {
    this->x = 0;
    y       = 0; // When it is not ambiguous, this-> can be
		 // removed. We will keep it in next code for the
		 // sake of clarity.
  }

  // This is grid_point_add. Note the keyword "const" before the
  // opening brace. In grid_point_add, the this_point argument was a
  // const reference, telling the compiler that it has to check that
  // we do not modify the this_point object. Here, as this argument
  // has gone and is an implicit "this" argument, there must be a
  // place somewhere to tell the compiler to check that it won't be
  // modified. Writting "const" before the opening brace is the way we
  // say it to the compiler.
  // grid_point_add is returning a new grid_point which is the sum of 
  // "this" with other_point and therefore, "this" is not modified. 
  grid_point add(const grid_point& other_point) const {
    fun_scope;
    grid_point res {this->x + other_point.x, this->y + other_point.y};
    return res;
  }
  
  grid_point sub(const grid_point& other_point) const {
    fun_scope;
    return {this->x - other_point.x, this->y - other_point.y};
  }

  num dot_product(const grid_point& other_point) const {
    fun_scope;
    return this->x * other_point.x + this->y * other_point.y;
  }

  // Have a look at grid_point_distance2 in the previous example, this
  // is the one we transform now. In this method, in the former
  // version, we called
  //
  //     grid_point tmp = grid_point_sub(this_point, other_point);
  //     return grid_point_dot_product(tmp, tmp);
  //
  // This should be replaced by
  //     grid_point tmp = grid_point::sub(this_point, other_point);
  //     return grid_point::dot_product(tmp, tmp);
  //
  // which are the new names of the functions now. Moreover, first
  // argument is now a pointer (this), and not a reference. For the
  // former "this_point", we already have the pointer "this" available
  // now. So let us modify the first line.
  //
  //     grid_point tmp = grid_point::sub(this, other_point);
  //     return grid_point::dot_product(tmp, tmp);
  //
  // For the second line, things are more tricky. Indeed, the first
  // argument of grid_point::dot_product is "this", and the above code
  // still considers that it is a reference since we pass tmp and not
  // &tmp. We should pass the address &tmp to put things right. This
  // is true for the first argument only, since the second argument
  // (the only one that you see in the new dot_product definition) is
  // still a reference.
  //
  //     grid_point tmp = grid_point::sub(this, other_point);
  //     return grid_point::dot_product(&tmp, tmp);
  //
  // As we are in the "struct grid_point {...}" section of code, the
  // namespace information "grid_point::" can be removed.
  //
  //     grid_point tmp = sub(this, other_point);
  //     return dot_product(&tmp, tmp);
  //
  // In object oriented language, passing the argument that plays the
  // role of the "this" pointer is not done as in the two previous
  // lines, but this is just a syntactic sugar replacing the above
  // writing. The notation "a.f(b,c)", when f is a method, means
  // "f(&a, b, c)". If ptr_a is a pointer, this means that you should
  // write "(*ptr_a).f(b,c)" in order to call "f(ptr_a, b c)". As for
  // attributes, the symbol "->" simplifies this, and you can write
  // "ptr_a->f(b,c)" in order to call "f(ptr_a, b c)". So finally, the
  // two lines rewrite as
  //
  //     grid_point tmp = this->sub(other_point);
  //     return tmp.dot_product(tmp);
  
  num distance2(const grid_point& other_point) const {
    fun_scope;
    grid_point tmp = this->sub(other_point); // this-> can also be omitted....
    return tmp.dot_product(tmp);
  }

  bool is_equal(const grid_point& other_point) const {
    fun_scope;
    return this->x == other_point.x && this->y == other_point.y;
  }

  bool is_different(const grid_point& other_point) const {
    fun_scope;
    return this->x != other_point.x || this->y != other_point.y;
  }

  // The next function that we will transform from previous example is
  // grid_point_zero, that has no const grid_point& this_point since
  // it has no argument at all. If we write
  //
  //     grid_point zero() {...
  //
  // It would mean that there is an implicit "this" argument, as if our
  // grid_point_zero function had a grid_point& this_point. We need to
  // tell the compiler not to consider the implicit this. This is done
  // by the keyword "static".
  static grid_point zero() {
    // using "this" in this part of the code has no meaning since that
    // method is declared static.
    return {num("0.x", 0), num("0.y", 0)};
    // Indeed, we do not need "this".
  }

  // grid_point_from_ints is also turned into a static method.
  static grid_point from_ints(int x, int y) {
    return {num("int.x", x), num("int.y", y)};
  }

  // We could have used a new constructor as follows, but here we
  // illustrate the use of a static function.
  //    grid_point(int x, int y) : x("int.x", x), y("int.y", y) {}
  
};

std::ostream& operator<<(std::ostream& os, const grid_point& the_one_to_print) {
  os << '(' << the_one_to_print.x << ", " << the_one_to_print.y << ')';
  return os;
}

// Ok, let us use it. You will see how the syntax gets lightened, this
// is the purpose of such syntactic sugar. This is also the flavor of
// object oriented programming... Did I tell you that we were already
// in the world of object oriented programming ?

int main(int argc, char* argv[]) {
  grid_point a {num("a.x", 10), num("a.y", 5)};
  grid_point b {-a.y, a.x};

  ___;

  rem("Testing distance");

  // We reuse the object oriented syntax. Calling
  //
  //     grid_point::distance2(&a, b)
  //
  // simplifies as
  //
  //     a.distance2(b);
  //
  // Since a is a variable declared with type grid_point, the compiler knows that
  // distance2 is indeed grid_point::distance2.
  
  num d2 = a.distance2(b);
  std::cout << scope_indent << "d2(" << a << ", " << b << ") = " << d2 << std::endl;
  
  ___;
  
  rem("Testing orthogonality");

  // std::boolapha prints the booleans as "true/false" rather than "0/1".
  bool ortho = (a.dot_product(b) == 0); // We have designed nums so that they do compare to integers.
  std::cout << scope_indent << "Are " << a << " and " << b << " orthogonal ? "
	    << std::boolalpha << ortho
	    << std::endl;

  ___;
	       
  rem("Testing from_ints");

  // The static method cannot be called with the a.f(x) form, since
  // with this form, a plays the role of "this", that is not existing
  // for static methods. We then call them as a usual function, keeping
  // in mind that their real name is not from_ints but
  // grid_point::from_ints.
  bool equal = a.is_equal(grid_point::from_ints(10, 5));
  // If we had defined a constructor instead of the static function
  // from_ints, the previous line would have been:
  //     bool equal = a.is_equal(grid_point(10, 5));
  
  std::cout << scope_indent << "This is supposed to be true, is it ? " << std::boolalpha
	    << equal
	    << std::endl;
  ___;

  rem("Testing raz");
  a.raz();
  equal = a.is_equal(grid_point::zero());
  std::cout << scope_indent << "This is supposed to be true, is it ? " << std::boolalpha
	    << equal
	    << std::endl;

  return 0;
}

