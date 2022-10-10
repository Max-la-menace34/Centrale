#include <iostream>
#include <iomanip>

/*

  The previous code, for some programmers, may be considered as messy,
  since the interface of the struct grid_point, i.e. the description of
  what you can do with it, i.e. the set of methods, is mixed up with the
  code of the methods. It could be nice to have only the interface in
  one hand, and the detail of implementation in the other hand.

  We won't do this in further tutorials, but let us show in this
  example how you can separate the interface from the code. We have
  already done this, when we declared function headers only and then,
  afterwards, we defined the functions. We will do the same for the
  struct grid_point.

*/


// ##################
// #                #
// # Header section #
// #                #
// ##################
//        ###
//        ###
//      #######
//       #####
//        ###
//         #


// This is the struct definition, without the code of the
// method. Doxygen comments are welcome here. Doxygen allows for
// generating a documentation from comments. This is not a feature of
// C++, it is a process that reads your files and finds keywords in
// the comments, finds what you declare, and build up web pages,
// latex documentation, and so on. see www.doxygen.org


/** 
 * This struct implement grid points, i.e. 2D points with integer
 * coordinates.
 */
struct grid_point {
  int x; //!< abscissa of the point.
  int y; //!< ordinate of the point.

  /**
   * Constructor from ints.
   */
  grid_point(int x, int y);

  /**
   * Copy constructor.
   */
  grid_point(const grid_point&);

  /**
   * Default constructor creates the (0,0) point.
   */ 
  grid_point();

  /**
   * Assignment.
   */
  grid_point& operator=(const grid_point&);

  /**
   * This method modifies the object so that is becomes (0, 0).
   */
  void raz();

  /**
   * \f$(a, b) + (c, d) \rightarrow (a+c, b+d)\f$
   */
  grid_point operator+(const grid_point&) const; 
  
  /**
   * \f$(a, b) - (c, d) \rightarrow (a-c, b-d)\f$
   */
  grid_point operator-(const grid_point&) const;

  /**
   * The * operator is the dot product: \f$ (a, b).(c, d) \rightarrow (a\times c + b\times d)\f$.
   */
  int operator*(const grid_point&) const;

  /**
   * This implements \f$d^2((a,b), (c, d)) = \left\| (a,b) - (c,d) \right\|^2\f$.
   */
  int distance2(const grid_point&) const;

  bool operator==(const grid_point&) const;
  bool operator!=(const grid_point&) const;

  /**
   * This is the (0, 0) constant.
   * @returns (0, 0).
   */
  static grid_point zero();
  
  /**
   * This is the (x, y) constant.
   * @param x the abscissa.
   * @param y the ordinate.
   * @returns (x, y).
   */
  static grid_point from_ints(int x, int y);
  
};

/**
 * grid points supports serialization into ostreams.
 */
std::ostream& operator<<(std::ostream&, const grid_point&);


//         #
//        ###
//       #####
//      #######
//        ###
//        ###
// ##################
// #                #
// # Header section #
// #                #
// ##################


// Only the code above, in the "header section", is necessary for
// compiling our main, i.e. for using the struct grid_point.


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


// ##########################
// #                        #
// # Implementation section #
// #                        #
// ##########################
//        ###
//        ###
//      #######
//       #####
//        ###
//         #


// here, we write the method and function codes. For the methods, we
// have to name them with their full name, since we are not in the
// namespace "struct grid_point {....}" anymore.


grid_point::grid_point(int x, int y)                  : x(x), y(y)                         {}

grid_point::grid_point(const grid_point& other_point) : x(other_point.x), y(other_point.y) {}

grid_point::grid_point()                              : x(0), y(0)                         {}
  
grid_point& grid_point::operator=(const grid_point& other_point) {
  x = other_point.x;
  y = other_point.y;
  return *this; 
}

void grid_point::raz() {
  this->x = 0;
  this->y = 0;
}

grid_point grid_point::operator+(const grid_point& other_point) const {
  grid_point res {this->x + other_point.x, this->y + other_point.y};
  return res;
}
  
grid_point grid_point::operator-(const grid_point& other_point) const {
  return {this->x - other_point.x, this->y - other_point.y};
}

int grid_point::operator*(const grid_point& other_point) const {
  return this->x * other_point.x + this->y * other_point.y;
}

int grid_point::distance2(const grid_point& other_point) const {
  grid_point tmp = *this - other_point; 
  return tmp * tmp;
}

bool grid_point::operator==(const grid_point& other_point) const {
  return this->x == other_point.x && this->y == other_point.y;
}

bool grid_point::operator!=(const grid_point& other_point) const {
  return this->x != other_point.x || this->y != other_point.y;
  // We could also have called the previous == operator:
  //     return !(*this == other_point);
}

// no static here, the compiler knows it from the header.
grid_point grid_point::zero() {
  return {0, 0}; 
}

// no static here, the compiler knows it from the header.
grid_point grid_point::from_ints(int x, int y) {
  return {x, y};
}
  
// This is not a method but an ordinary function, outside any struct
// definition. So no grid_point:: prefix is necessary. It would be
// wrong.
std::ostream& operator<<(std::ostream& os, const grid_point& the_one_to_print) {
  os << '(' << the_one_to_print.x << ", " << the_one_to_print.y << ')';
  return os;
}

//         #
//        ###
//       #####
//      #######
//        ###
//        ###
// ##########################
// #                        #
// # Implementation section #
// #                        #
// ##########################


/*

  When you separate the header and the implementation, the idea is to
  have separate compiling of your source files. Indeed, for bigger
  developpements, it can be a good idea to have a file for each type
  you define, rather than having huge main codes.

  In this very classical case (almost standard), you write the header
  section in a file named "grid_point.hpp". You could call it with
  another name, like "graphical_acceleration.hpp", or even "foo.docx",
  the compiler does not care about file namings, but such design would
  obfuscate the reading of your code. 

  As cpp means C++, hpp means "header of C++". Instead of copy-pasting
  this header code in your test.cpp file when you need it for
  compiling the main, rather use 
      #include "grid_point.hpp"
  where you would like the header section to be copied. This is what 
  the #include directive we put at the beginning of our C++ files 
  from the very beginning of this tutorial actually do !

  Write the implementation section in a "grid_point.cpp" file. Start
  the code of this file by including grid_point.hpp, since the
  compiler needs to know things about the grid_point type in order to
  understand and check the function description (e. g. mentionning an
  attribute z would be an error...).

  So at the end, you will have 3 files : 
  - main.cpp : It contains the code of the main function only, 
               preceeded by the required #include directives.
  - grid_point.hpp : This is the header for using grid_points.
  - grid_point.cpp : This describes the "how to" for grid_point.

  Compilation requires compiling separately all the .cpp files, and
  then link the generated binary files together. This is out of the
  scope of that tutorial and we will discuss this during our lectures.

*/
