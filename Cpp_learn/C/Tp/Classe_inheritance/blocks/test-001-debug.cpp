#include <iostream>

#include <blk.hpp>

int main(int argc, char* argv[]) {
  blk::expr e1 = blk::debug('a', 7, 5, 4, 2);
  auto      e2 = blk::debug('b', 8, 3, 4, 1);

  std::cout << e1 << std::endl
	    << e2 << std::endl;
  return 0;
}

/* 
   Output :
				 
  -2 :   aaaaaaa
  -1 :   aaaaaaa
   0 :   aaaa#aa
   1 :   aaaaaaa
   2 :   aaaaaaa

  -1 :   bbbbbbbb
   0 :   bbbb#bbb
   1 :   bbbbbbbb

*/
