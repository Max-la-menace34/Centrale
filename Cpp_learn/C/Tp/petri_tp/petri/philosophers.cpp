#include <vector>
#include <iterator>
#include <string>
#include <stdexcept>
#include <array>
#include <algorithm>
#include <philosophers.hpp>

std::array<petri::Transition, 3> make_one(unsigned int philosopher, petri::pool_ref left_fork, petri::pool_ref right_fork) {
  std::string philo_name = std::string("ph. ") + std::to_string(philosopher);
  auto thinking = petri::make_pool(philo_name + " thinking",       1);
  auto got_one  = petri::make_pool(philo_name + " has one fork",   0);
  auto got_both = petri::make_pool(philo_name + " has both forks", 0);

  // Tansitions:                  --- name --------------------   --- inputs ---------------------  --- outputs ------------------------------------
  petri::Transition takes_first  {(philo_name + " takes first" ), {{left_fork,  1}, {thinking, 1}}, {{got_one,  1}}                                 };
  petri::Transition takes_second {(philo_name + " takes second"), {{right_fork, 1}, {got_one,  1}}, {{got_both, 1}}                                 };
  petri::Transition eats         {(philo_name + " eats"        ), {{got_both,   1}}               , {{thinking, 1}, {left_fork, 1}, {right_fork, 1}}};
  
  return {takes_first, takes_second, eats};
}

petri::Network philo::make(unsigned int nb_philosophers, unsigned int seed) {
  if(nb_philosophers < 2) throw std::logic_error("Not enough philosophers");
  
  std::vector<petri::pool_ref> forks;
  auto fork_out = std::back_inserter(forks);
  
  for(unsigned int i = 0; i < nb_philosophers; ++i)
    *(fork_out++) = petri::make_pool(std::string("fork ") + std::to_string(i), 1);
  // We loop, putting the first pool at the end.
  *(fork_out++) = *(forks.begin());
  
  petri::Network res(seed);
  
  auto left_it  = forks.begin();
  auto right_it = left_it + 1;
  for(unsigned int i = 0; i < nb_philosophers; ++i) {
    auto transitions = make_one(i, *(left_it++), *(right_it++));
    std::move(transitions.begin(), transitions.end(), std::back_inserter(res.transitions));
  }

  return res;
}
