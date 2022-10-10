#pragma once

#include <utility> // for std::pair
#include <vector>
#include <string>

#include <petriPool.hpp> // For talking about pools.

namespace petri {
  using slot = std::pair<pool_ref, unsigned int>;
 
  struct Transition {
    std::string       name;
    std::vector<slot> inputs;
    std::vector<slot> outputs;

    bool activate();
    bool is_activable() const;
    operator bool() const;
    void  operator()();
  };
  std::ostream& operator<<(std::ostream& os, const Transition& t);
}
