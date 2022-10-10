#pragma once

#include <petri.hpp>

namespace philo {
  petri::Network make(unsigned int nb_philosophers, unsigned int seed);
}
