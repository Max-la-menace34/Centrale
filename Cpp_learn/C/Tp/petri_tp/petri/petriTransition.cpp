#include <petriTransition.hpp>

// To be done


bool petri::Transition::is_activable()const {
    for (auto& i : inputs){
        if(i.first<i.second)
        return false;
    }
    return true;
}

bool petri::Transition::activate(){
    /*
    On a un shared pointer dans notre cas : first(name,nombre de token), second element ( nombre de token nécessaire)
    */
    for(auto& i :inputs ){ i.first->nb_tokens -= i.second;}
    for(auto& j : outputs){j.first->nb_tokens += j.second;}
    return true;
}
void petri::Transition::operator()() {
  activate();
}
petri::Transition::operator bool () const {
  return is_activable();
}
std::ostream& petri::operator<<(std::ostream& os, const petri::Transition& t){
    os<<"{" ; for(auto& i : t.inputs){os<<""<<i.first<<"/"<<i.second<<" ";};
    os<< " --> "<<t.name<<" --> "; for (auto& j :t.outputs){os<<""<<j.first<<"/"<<j.second<<" ";};
    os<<"}";
    return os;
}
