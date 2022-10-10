#include <philosophers.hpp>
#include <petri.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
  unsigned int nb_philosophers;
  std::cout << "Enter the number of philosophers (>= 2): " << std::flush;
  std::cin >> nb_philosophers;

  if(nb_philosophers < 2) nb_philosophers = 2;

  auto network = philo::make(nb_philosophers, 0);
  std::ofstream file(std::to_string(nb_philosophers) + "-graph.dot");
  file << network;
  
  return 0;
}
