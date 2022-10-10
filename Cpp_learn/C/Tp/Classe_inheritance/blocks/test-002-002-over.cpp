#include <iostream>

#include <blk.hpp>

int main(int argc, char* argv[]) {
  
  auto a = blk::debug('a', 7, 5, 4, 3);
  auto b = blk::debug('b', 6, 8, 1, 2);
  auto c = blk::debug('c', 3, 3, 1, 1);

  std::cout << blk::over(a, b)               << std::endl
	    << blk::over(b, a)               << std::endl
	    << blk::over(c, b)               << std::endl
	    << blk::over(b, c)               << std::endl
	    << blk::over(b, blk::over(c, a)) << std::endl;
  
  return 0;
}

/*
   Output :

  -3 :   aaaaaaa  
  -2 :   aaaaaaa  
  -1 :   aaaaaaa  
   0 :   aaaa#aa  
   1 :   aaaaaaa  
   2 :      bbbbbb
   3 :      bbbbbb
   4 :      b#bbbb
   5 :      bbbbbb
   6 :      bbbbbb
   7 :      bbbbbb
   8 :      bbbbbb
   9 :      bbbbbb

  -2 :      bbbbbb
  -1 :      bbbbbb
   0 :      b#bbbb
   1 :      bbbbbb
   2 :      bbbbbb
   3 :      bbbbbb
   4 :      bbbbbb
   5 :      bbbbbb
   6 :   aaaaaaa  
   7 :   aaaaaaa  
   8 :   aaaaaaa  
   9 :   aaaa#aa  
  10 :   aaaaaaa  

  -1 :   ccc   
   0 :   c#c   
   1 :   ccc   
   2 :   bbbbbb
   3 :   bbbbbb
   4 :   b#bbbb
   5 :   bbbbbb
   6 :   bbbbbb
   7 :   bbbbbb
   8 :   bbbbbb
   9 :   bbbbbb

  -2 :   bbbbbb
  -1 :   bbbbbb
   0 :   b#bbbb
   1 :   bbbbbb
   2 :   bbbbbb
   3 :   bbbbbb
   4 :   bbbbbb
   5 :   bbbbbb
   6 :   ccc   
   7 :   c#c   
   8 :   ccc   

  -2 :      bbbbbb
  -1 :      bbbbbb
   0 :      b#bbbb
   1 :      bbbbbb
   2 :      bbbbbb
   3 :      bbbbbb
   4 :      bbbbbb
   5 :      bbbbbb
   6 :      ccc   
   7 :      c#c   
   8 :      ccc   
   9 :   aaaaaaa  
  10 :   aaaaaaa  
  11 :   aaaaaaa  
  12 :   aaaa#aa  
  13 :   aaaaaaa  
*/
