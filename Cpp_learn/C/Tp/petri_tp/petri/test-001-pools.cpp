#include <iostream>
#include <iomanip>
#include <petri.hpp>

int main(int argc, char* argv[]) {

  auto p1 = petri::make_pool("red", 10);
  auto p2 = petri::make_pool("green", 5);
  auto p3 = petri::make_pool("orange", 123);

  std::cout << p1 << std::endl
	    << p2 << std::endl
	    << p3 << std::endl;
  /* 
     Output expected :
     (red, 10)
     (green, 5)
     (orange, 123)
  */

  unsigned int nb_tokens_required = 10;
  std::cout << std::boolalpha
	    << (p1 < nb_tokens_required) << ' '
	    << (p2 < nb_tokens_required) << ' '
	    << (p3 < nb_tokens_required) << std::endl;
  /*
    Output expected :
    false true false
  */
  
  return 0;
}
