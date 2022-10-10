#include <iostream>
#include <petri.hpp>
#include <iterator>
#include <random>
#include <fstream>

int main(int argc, char* argv[]) {
  std::random_device rd;
  petri::Network redlight(rd()); // rd() gets a random seed.
  
  // Here are the pools

  auto red_1    = petri::make_pool("red-1",    1);
  auto green_1  = petri::make_pool("green-1",  0);
  auto orange_1 = petri::make_pool("orange-1", 0);
  auto red_2    = petri::make_pool("red-2",    1);
  auto green_2  = petri::make_pool("green-2",  0);
  auto orange_2 = petri::make_pool("orange-2", 0);
  auto all_red  = petri::make_pool("all-red",  1);

  // Let us add transitions
  
  auto out_trans_iter = std::back_inserter(redlight.transitions);
  
  *(out_trans_iter++) = {"1 turns green",
			 {{red_1,    1}, {all_red, 1}},
			 {{green_1,  1}}};
  *(out_trans_iter++) = {"1 turns orange",
			 {{green_1,  1}},
			 {{orange_1, 1}}};
  *(out_trans_iter++) = {"1 turns red",
			 {{orange_1, 1}},
			 {{red_1,    1}, {all_red, 1}}};
  
  *(out_trans_iter++) = {"2 turns green",
			 {{red_2,    1}, {all_red, 1}},
			 {{green_2,  1}}};
  *(out_trans_iter++) = {"2 turns orange",
			 {{green_2,  1}},
			 {{orange_2, 1}}};
  *(out_trans_iter++) = {"2 turns red",
			 {{orange_2, 1}},
			 {{red_2,    1}, {all_red, 1}}};

  // Let us run the sumulation
  redlight.transition_verbosity = true;
  for(unsigned int nb_steps = 0; (nb_steps < 100) && redlight() ; ++nb_steps);

  // Let us generate a .dot file
  std::ofstream file("redlight.dot");
  file << redlight;
  
  return 0;
}

