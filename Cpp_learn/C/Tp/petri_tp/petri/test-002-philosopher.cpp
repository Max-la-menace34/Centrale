#include <philosophers.hpp>
#include <petri.hpp>

#include <iostream>
#include <fstream>

double average(unsigned int nb_philosophers, unsigned int nb_samples, unsigned int seed) {
  double sum = 0;
  std::mt19937 gen(seed); 
  for(unsigned int sample = 0; sample < nb_samples; ++sample) {
    auto network = philo::make(nb_philosophers, gen());
    while(network()) ++sum;
  }
  return sum / nb_samples;
}

#define MIN 2
#define MAX 100
int main(int argc, char* argv[]) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::ofstream file("dead-lock.py");

  std::cout << "Generating \"dead-lock.py\"..." << std::endl;
  file << "import matplotlib.pyplot as plt" << std::endl
       << "import numpy as np" << std::endl
       << std::endl
       << "Y = np.array([" << average(MIN, 20, gen());
  for(unsigned int i = MIN + 1; i <= MAX; ++i) file << ", " << average(i, 20, gen());
  file << "])" << std::endl
       << std::endl
       << "plt.figure()" << std::endl
       << "plt.title('nb steps before deadlock')" << std::endl
       << "plt.xlabel('nb philosophers')" << std::endl
       << "plt.scatter(range(" << MIN << ", " << MAX + 1 << "), Y)" << std::endl
       << "plt.show()" << std::endl;
  std::cout << "... done." << std::endl;
  return 0;
}
