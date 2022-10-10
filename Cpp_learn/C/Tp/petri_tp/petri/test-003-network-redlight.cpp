#include <iostream>
#include <petri.hpp>
#include <iterator>
#include <random>
#include <fstream>

int main(int argc, char* argv[]) {
  std::random_device rd;
  petri::Network redlight(rd()); // rd() gets a random seed.
  
  // Here are the pools

  auto red    = petri::make_pool("red",    1);
  auto green  = petri::make_pool("green",  0);
  auto orange = petri::make_pool("orange", 0);

  // Let us add transitions
  
  auto out_trans_iter = std::back_inserter(redlight.transitions);
  *(out_trans_iter++) = {"turns green",
			 {{red,    1}},
			 {{green,  1}}};
  *(out_trans_iter++) = {"turns orange",
			 {{green,  1}},
			 {{orange, 1}}};
  *(out_trans_iter++) = {"turns red",
			 {{orange, 1}},
			 {{red, 1}}};

  // Let us run the sumulation
  redlight.transition_verbosity = true;
  for(unsigned int nb_steps = 0; (nb_steps < 10) && redlight() ; ++nb_steps);
  std::ofstream file("redlight.dot");
  file << redlight; // redlight is a petri::Network
  /*
     Output expected :
     turns green
     turns orange
     turns red
     turns green
     turns orange
     turns red
     turns green
     turns orange
     turns red
     turns green

  */

  return 0;
}

