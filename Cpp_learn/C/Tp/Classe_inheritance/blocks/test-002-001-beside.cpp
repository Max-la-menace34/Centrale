#include <iostream>

#include <blk.hpp>

int main(int argc, char* argv[]) {
  
  auto a = blk::debug('a', 7, 5, 4, 3);
  auto b = blk::debug('b', 4, 8, 1, 2);
  auto c = blk::debug('c', 3, 3, 1, 1);

  std::cout << blk::beside(a, b)                 << std::endl
	    << blk::beside(b, a)                 << std::endl
	    << blk::beside(c, b)                 << std::endl
	    << blk::beside(b, c)                 << std::endl
	    << blk::beside(b, blk::beside(c, a)) << std::endl;
  
  return 0;
}

/*
  Output:

  -3 :   aaaaaaa    
  -2 :   aaaaaaabbbb
  -1 :   aaaaaaabbbb
   0 :   aaaa#aab#bb
   1 :   aaaaaaabbbb
   2 :          bbbb
   3 :          bbbb
   4 :          bbbb
   5 :          bbbb

  -3 :       aaaaaaa
  -2 :   bbbbaaaaaaa
  -1 :   bbbbaaaaaaa
   0 :   b#bbaaaa#aa
   1 :   bbbbaaaaaaa
   2 :   bbbb       
   3 :   bbbb       
   4 :   bbbb       
   5 :   bbbb       

  -2 :      bbbb
  -1 :   cccbbbb
   0 :   c#cb#bb
   1 :   cccbbbb
   2 :      bbbb
   3 :      bbbb
   4 :      bbbb
   5 :      bbbb

  -2 :   bbbb   
  -1 :   bbbbccc
   0 :   b#bbc#c
   1 :   bbbbccc
   2 :   bbbb   
   3 :   bbbb   
   4 :   bbbb   
   5 :   bbbb   

  -3 :          aaaaaaa
  -2 :   bbbb   aaaaaaa
  -1 :   bbbbcccaaaaaaa
   0 :   b#bbc#caaaa#aa
   1 :   bbbbcccaaaaaaa
   2 :   bbbb          
   3 :   bbbb          
   4 :   bbbb          
   5 :   bbbb          

*/
