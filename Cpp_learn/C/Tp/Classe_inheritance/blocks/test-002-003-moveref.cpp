#include <iostream>

#include <blk.hpp>

int main(int argc, char* argv[]) {
  auto a = blk::debug('a', 3, 3, 1, 1);
  auto b = blk::debug('b', 5, 5, 2, 2);
  auto c = blk::move_ref(b, 0, 0);

  std::cout << blk::beside(a,b) << std::endl
	    << blk::over  (a,b) << std::endl
	    << blk::beside(a,c) << std::endl
	    << blk::over  (a,c) << std::endl;
  
  return 0;
}

/*
   Output:

  -2 :      bbbbb
  -1 :   aaabbbbb
   0 :   a#abb#bb
   1 :   aaabbbbb
   2 :      bbbbb

  -1 :    aaa 
   0 :    a#a 
   1 :    aaa 
   2 :   bbbbb
   3 :   bbbbb
   4 :   bb#bb
   5 :   bbbbb
   6 :   bbbbb

  -1 :   aaa     
   0 :   a#abbbbb
   1 :   aaabbbbb
   2 :      bb#bb
   3 :      bbbbb
   4 :      bbbbb

  -1 :   aaa   
   0 :   a#a   
   1 :   aaa   
   2 :    bbbbb
   3 :    bbbbb
   4 :    bb#bb
   5 :    bbbbb
   6 :    bbbbb

*/
