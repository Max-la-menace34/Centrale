#include <petriPool.hpp>

petri::Pool::Pool(const std::string& name, unsigned int nb_tokens)
  : name(name), nb_tokens(nb_tokens) {}

petri::pool_ref petri::make_pool(const std::string& name, unsigned int nb_tokens) {
  return std::make_shared<Pool>(name, nb_tokens);
}

std::ostream& petri::operator<<(std::ostream& os, petri::pool_ref p) {
  os << '(' << p->name << ", " << p->nb_tokens << ')';
  return os;
}

bool petri::operator<(petri::pool_ref p, unsigned int required_number_of_tokens) {
  return p->nb_tokens < required_number_of_tokens;
}
