#include <iostream>
#include <iomanip> // for std::boolapha

/*

  This example is a rewriting of the previous example, but without
  using num. We use int instead, as a C++ programmer not concerned by
  tracing the life of the data in memory would do.

  Nothing new there... But the idea is that you see that in usual
  cases, things are not that complicated once you understand the
  underlying mechanisms.

  With nums, we used to write "const num& x" for the arguments that we
  only need for reading, avoiding a copy. As now we use ints, that are
  only four bytes, less than a pointer on a 64bit machine, passing
  references, i.e. a pointer that is the address of an int, is a loss
  of time. For reading, passing the int arguments by copy rather than
  by reference is a good idea.

 */

struct grid_point {
  int x;
  int y;
  
  grid_point(int x, int y)                  : x(x), y(y)                         {}
  grid_point(const grid_point& other_point) : x(other_point.x), y(other_point.y) {}
  grid_point()                              : x(0), y(0)                         {}
  
  grid_point& operator=(const grid_point& other_point) {
    x = other_point.x;
    y = other_point.y;
    return *this; 
  }

  void raz() {
    this->x = 0;
    this->y = 0;
  }

  grid_point operator+(const grid_point& other_point) const {
    grid_point res {this->x + other_point.x, this->y + other_point.y};
    return res;
  }
  
  grid_point operator-(const grid_point& other_point) const {
    return {this->x - other_point.x, this->y - other_point.y};
  }

  int operator*(const grid_point& other_point) const {
    return this->x * other_point.x + this->y * other_point.y;
  }

  int distance2(const grid_point& other_point) const {
    grid_point tmp = *this - other_point; 
    return tmp * tmp;
  }

  bool operator==(const grid_point& other_point) const {
    return this->x == other_point.x && this->y == other_point.y;
  }

  bool operator!=(const grid_point& other_point) const {
    return this->x != other_point.x || this->y != other_point.y;
  }

  static grid_point zero() {
    return {0, 0}; 
  }

  static grid_point from_ints(int x, int y) {
    return {x, y};
  }
  
};

std::ostream& operator<<(std::ostream& os, const grid_point& the_one_to_print) {
  os << '(' << the_one_to_print.x << ", " << the_one_to_print.y << ')';
  return os;
}

// Now, the code gets much smarter.

int main(int argc, char* argv[]) {
  grid_point a {  10,   5};
  grid_point b {-a.y, a.x};

  
  int d2 = a.distance2(b);
  std::cout << "d2(" << a << ", " << b << ") = " << d2 << std::endl;

  std::cout << "Are " << a << " and " << b << " orthogonal ? "
	    << std::boolalpha << (a * b == 0) 
	    << std::endl;

  std::cout << "This is supposed to be true, is it ? " << std::boolalpha
	    << (a == grid_point::from_ints(10, 5))
	    << std::endl;
  a.raz();
  std::cout << "This is supposed to be true, is it ? " << std::boolalpha
	    << (a == grid_point::zero())
	    << std::endl;

  return 0;
}

