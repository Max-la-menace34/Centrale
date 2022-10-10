#include <iostream>
#include <petri.hpp>

int main(int argc, char* argv[]) {
  auto p1 = petri::make_pool("A", 10);
  auto p2 = petri::make_pool("B", 10);
  auto p3 = petri::make_pool("C", 10);
  auto p4 = petri::make_pool("D", 10);
  auto p5 = petri::make_pool("E", 10);

  petri::Transition t {"T",
		       {{p1, 6}, {p2, 7}, {p3, 1}},
		       {{p1, 4}, {p4, 1}, {p5, 9}}};

  std::cout << t << std::endl;
  /* 
     Output expected :
     {(A, 10)/6 (B, 10)/7 (C, 10)/1 --> T --> (A, 10)/4 (D, 10)/1 (E, 10)/9}
  */

  if(t) {
    // We have to get here
    t();
  }
  
  std::cout << t << std::endl;
  /* 
     Output expected :
     {(A, 8)/6 (B, 3)/7 (C, 9)/1 --> T --> (A, 8)/4 (D, 11)/1 (E, 19)/9}
  */

  if(!t) {
    // We have to get here
    std::cout << "The transition cannot be activated anymore" << std::endl;
  }

  return 0;
}
