#include <petriTransition.hpp>


bool petri::Transition::is_activable() const {
  for(auto& s : inputs)
    if(s.first < s.second)
      return false;
  return true;
  
// Other version with the structured bindings.
/*
  for(auto [p, nb_needed] : inputs)
    if(p < nb_needed)
      return false;
  return true;
*/
}


petri::Transition::operator bool () const {
  return is_activable();
}

void petri::Transition::activate() {
  for(auto& s : inputs)  s.first->nb_tokens -= s.second;
  for(auto& s : outputs) s.first->nb_tokens += s.second;
  
  // Other version with the structured bindings.
  /*
    for(auto [p, nb_needed]: inputs)    p->nb_tokens -= nb_needed;
    for(auto [p, nb_produced]: outputs) p->nb_tokens += nb_produced;
   */
}

void petri::Transition::operator()() {
  activate();
}

std::ostream& petri::operator<<(std::ostream& os, const petri::Transition& t) {
  os << '{';
  for(auto& s : t.inputs) os << s.first << '/' << s.second << ' ';
  os << "--> " << t.name << " -->";
  for(auto& s : t.outputs) os << ' ' << s.first << '/' << s.second;
  os << '}';
  return os;
}
