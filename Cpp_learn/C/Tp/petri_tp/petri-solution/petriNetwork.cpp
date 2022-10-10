#include <petriNetwork.hpp>
#include <iterator>
#include <map>

petri::Network::Network(unsigned int seed)
  : gen(seed), transitions(), transition_verbosity(false) {
}

bool petri::Network::operator()() {
  std::vector<Transition*> activables;
  auto out = std::back_inserter(activables);
  for(auto& t : transitions)
    if(t) *(out++) = &t;

  if(activables.size() == 0)
    return false;

  auto id = std::uniform_int_distribution<std::size_t>(0, activables.size()-1)(gen);
  Transition& t = *(activables[id]);
  if(transition_verbosity) std::cout << t.name << std::endl;
  t();
  return true;
}

std::ostream& petri::operator<<(std::ostream& os, const petri::Network& n) {
  // A shared pointer type as pool_ref can be used as a map key, since
  // it has the required ordering operations (only < is required indeed).
  std::map<pool_ref, unsigned int> idf_of_pool;
  
  unsigned int next_free_pool_index = 0;
  unsigned int transition_index;

  os << "digraph Petri {" << std::endl
     << std::endl
     << "  overlap=scale" << std::endl
     << std::endl
     << "  /* nodes */" << std::endl
     << std::endl;

  transition_index = 0;
  for(auto& t : n.transitions) {
    os << "  transition" << (transition_index++) << " [label=\"" << t.name
       << "\", shape=box, color=red, style=bold];" << std::endl;
    for(auto& slot : t.inputs)
      if(auto it = idf_of_pool.find(slot.first); it == idf_of_pool.end()) // the pool is not yet associated to an idf.
	idf_of_pool[slot.first] = next_free_pool_index++;
    for(auto& slot : t.outputs)
      if(auto it = idf_of_pool.find(slot.first); it == idf_of_pool.end()) 
	idf_of_pool[slot.first] = next_free_pool_index++;
  }
  
  os << std::endl;
  for(auto& key_value_pair : idf_of_pool)
    os << "  pool" << key_value_pair.second << " [label=\"" << key_value_pair.first->name
       << "\", color=blue, style=bold];" << std::endl;
    
  
  os << std::endl
     << "  /* edges */" << std::endl
     << std::endl;
  transition_index = 0;
  for(auto& t : n.transitions) {
    for(auto& slot : t.inputs)
      os << "  pool" << idf_of_pool[slot.first] << " -> transition" << transition_index << " [label=\"" << slot.second << "\"]" << std::endl;
    for(auto& slot : t.outputs)
      os << "  transition" << transition_index << " -> pool" << idf_of_pool[slot.first] << " [label=\"" << slot.second << "\"]" << std::endl;

    ++transition_index;
  }
  os << '}' << std::endl;
  
  return os;
}
