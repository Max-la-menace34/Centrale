#include <iostream>

#include <blk.hpp>

int main(int argc, char* argv[]) {
  auto a = blk::text("Default is centered");
  auto b = blk::text("Aligned to the left"  , blk::align::left );
  auto c = blk::text("Aligned to the middle", blk::align::center);
  auto d = blk::text("Aligned to the right" , blk::align::right );

  std::cout << blk::over(blk::over(a, b), blk::over(c, d)) << std::endl;

  return 0;
}

/*
   Output :

   0 :             Default is centered         
   1 :                      Aligned to the left
   2 :            Aligned to the middle        
   3 :   Aligned to the right   

 */
