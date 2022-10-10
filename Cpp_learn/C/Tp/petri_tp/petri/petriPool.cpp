#include <petriPool.hpp>
#include <vector>

// To be done
petri::Pool::Pool(const std::string& name, unsigned int nb_tokens)
  : name(name), 
    nb_tokens(nb_tokens){}


petri::pool_ref petri::make_pool(const std::string& name, unsigned int nb_tokens){    
    //Pool p{name,nb_tokens};        inutile pour ce cas, il refait une recopie sur la stack prend plus de place 
    //return p;
    return std::make_shared<Pool>(name,nb_tokens);
}

std::ostream& petri::operator<<(std::ostream& os, pool_ref p){    
    return std::cout<<"("<<p -> name <<","<<p-> nb_tokens<<")";
}

// Compares the number of tokens to a number
bool petri::operator<(pool_ref p, unsigned int required_number_of_tokens){
    /**
     * Si ton p_value inférieur à requis alors return true
     */
    return (p->nb_tokens)< required_number_of_tokens;
}