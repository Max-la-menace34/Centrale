#include <iostream>

#include <blk.hpp>

int main(int argc, char* argv[]) {
  auto x = blk::text("x");
  auto y = blk::text("y");
  auto z = blk::text("z");


  // Note that the same blocks are used several times.
  std::cout << (  (y /(z + x)) == ((z + x / y) / (x + y + z) + 3.14) / (x + z / (y + 3)) == x  )
	    << std::endl;
  return 0;
}

/*
   Output:

  -5 :                     x               
  -4 :                 z + -               
  -3 :                     y               
  -2 :              ----------- + 3.14     
  -1 :      y        x + y + z             
   0 :   ------- = -------------------- = x
   1 :    z + x                z           
   2 :                  x + -------        
   3 :                       y + 3     

*/
