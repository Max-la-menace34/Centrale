#pragma once
#include <iostream>
#include <iomanip>
#include <functional>
#include <memory>
#include <list>
#include <tuple>
#include <map>
 
using namespace std::placeholders;

namespace gate {
  using action_type = std::function<void (void)>;
  using ref_action = std::shared_ptr<action_type>;
  using weak_action = std::weak_ptr<action_type>;
  using wire_type = std::pair<bool, std::list<weak_action>>;
  using ref_wire = std::shared_ptr<wire_type>;
 
  // Write all your code here
  void ____(){std::cout<<"+-----------------+---------+-----+"<<std::endl;}
  void ____display(double time,const std::string  name, bool  status){
    std::cout.precision(2);   // Sets the number of digits after '.' when floats are printed.
    std::cout<<"|";
    std::cout << std::left  << std::setw(17) << name;
    std::cout << std::left << std::setw(2) << "|"; 
    std::cout << std::right << std::setw(7) << time;                  // Writes foo******* (* represents a printed space).
    std::cout << std::right << std::setw(2) << "|";
    std::cout << std::right << std::fixed << std::setw(5) << (status? "on":"off");
    std::cout.precision(2); 
    std::cout<<"|"<<std::endl;
    }
    /*
    Wire:

    Return the sharedpointer(bool , list(weak_pointer))
    */
    auto wire(){
        return std::make_shared<wire_type>(false, std::list<weak_action>());
    }
        /*
    exectute:

    shared function action 
    */
     void execute(ref_action ref_a){
      (*ref_a)();
    }
    /*
    value:

    Return the status
    */
    bool value(ref_wire wire){
      return (*wire).first;
      }

        /*
    action:

    return shared pointer function of type void 
    */
    ref_action action(action_type f){
        return std::make_shared<action_type>(f);
    }
            /*
    set:

    Change the status of our wire if bools are different
    execute the list of action from our wire wich is a pair of bool and weak_pointer 

    */
    auto set(ref_wire wire, bool status ){
      if(wire ->first==status){return;}
        wire->first= status;
    auto& actions = wire->second;
    for(auto it = actions.begin(); it != actions.end();)
      if(auto a = it->lock(); a) {execute(a); ++it;}
      else {it = actions.erase(it);};
    }
    void link(ref_wire wire, ref_action action){(*wire).second.push_back(action);}

    using wire_agenda = std::multimap<double, std::tuple<ref_wire, bool>>;
    using ref_agenda = std::shared_ptr<std::tuple<double, wire_agenda>>;
    
    auto agenda() {return std::make_shared<std::tuple<double, wire_agenda>>(0,wire_agenda());}
    inline double now(ref_agenda a) {return std::get<0>(*a);}
    inline void notify(ref_agenda a, ref_wire w, bool status, double delay) {std::get<1>(*a).insert({now(a) + delay, {w, status}});}
  
    inline bool next(ref_agenda agenda ){
      auto& requests = std::get<1>(*agenda);
      if(requests.size()==0){return false;}
      auto next_it = requests.begin();
      auto [time,request] = *next_it;
      requests.erase(next_it);
      std::get<0>(*agenda) = time;
      set(std::get<0>(request),std::get<1>(request));
      return true;
    }
    inline void flush(ref_agenda a) {while(next(a));}
    inline void  flush(ref_agenda agenda, double number ){
      auto& requests = std::get<1>(*agenda);
      int compteur =0;
      while(requests.size() !=0 && compteur<number){
        next(agenda);
        compteur +=1;
      }
    }

    inline auto PROBE(ref_agenda agenda,const std::string& name , ref_wire wire ){
      auto act = action([agenda, name, wire](){____display(now(agenda), name, value(wire));});
      link(wire, act);
      return act;
    }

    inline auto AND(ref_agenda agenda, ref_wire a,ref_wire b ,ref_wire c ){
      auto act = action([agenda, a, b, c](){notify(agenda, c, value(a) && value(b), .5);});
      link(a, act);
      link(b, act);
      execute(act);
      return act;
    }
    inline auto OR(ref_agenda agenda, ref_wire a,ref_wire b ,ref_wire c ){
      auto act = action([agenda, a, b, c](){notify(agenda, c, value(a) || value(b), .5);});
      link(a, act);
      link(b, act);
      execute(act);
      return act;
    
    }
    auto NOT(ref_agenda a, ref_wire in, ref_wire out) {
      auto act = action([a, in, out](){notify(a, out, !value(in), .1);});
      link(in, act);
      execute(act); // Sets the effect on the gate plugging on the output wire.
      return act;

  }
  inline auto XOR(ref_agenda a, ref_wire in_1, ref_wire in_2, ref_wire out) {
    // Internal actions will handle these wires into their
    // closures... so they will survive to the exit of this code.
    auto w1 = wire();
    auto w2 = wire();
    auto w3 = wire();
    auto w4 = wire();

    auto g1 = NOT(a, in_1, w1     );
    auto g2 = NOT(a, in_2, w2     );
    auto g3 = AND(a, in_1, w2, w3 );
    auto g4 = AND(a, in_2, w1, w4 );
    auto g5 = OR (a, w3  , w4, out);

    // Keeping gates alive in the closure is the trick
    // for integrated circuits.
    return action([g1, g2, g3, g4, g5]() {});
  }


}
