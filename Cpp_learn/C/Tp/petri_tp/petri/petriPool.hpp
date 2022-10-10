#pragma once

#include <string>
#include <memory>
#include <iostream>

namespace petri {
  struct Pool {
    std::string  name;      // The name of the pool
    unsigned int nb_tokens; // The number of tokens in the pool.
    Pool(const std::string& name, unsigned int nb_tokens);
  };

  // A given pool may be referenced in several transitions, and it
  // will be stored in a global pool list at network side. This is why
  // we allocate a pool ion the heap once, and then we handle this
  // instance by share pointers.
  using pool_ref = std::shared_ptr<Pool>;
  pool_ref make_pool(const std::string& name, unsigned int nb_tokens);

  // Prints the pool
  std::ostream& operator<<(std::ostream& os, pool_ref p);

  // Compares the number of tokens to a number
  bool operator<(pool_ref p, unsigned int required_number_of_tokens);
}
