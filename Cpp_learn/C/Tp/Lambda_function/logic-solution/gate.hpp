#pragma once

#include <iostream>
#include <iomanip>
#include <functional>
#include <memory>
#include <list>
#include <tuple>
#include <map>

#define gate_NAME_WIDTH 15
#define gate_TIME_WIDTH  7

namespace gate {

  inline void ____() {
    std::cout << "+-"  << std::string(gate_NAME_WIDTH, '-')
	      << "-+-" << std::string(gate_TIME_WIDTH, '-')
	      << "-+-----+" << std::endl;
  }

  inline void ____display(double time, const std::string& name, bool status) {
    std::cout.precision(2);
    std::cout << "| " << std::left << std::setw(gate_NAME_WIDTH) << name
	      << " | " << std::right << std::fixed << std::setw(gate_TIME_WIDTH) << time;
    if(status) std::cout << " | on  |" << std::endl;
    else       std::cout << " | off |" << std::endl;
  }

  using ref_action  = std::shared_ptr<std::function<void ()>>;
  using wref_action = std::weak_ptr<std::function<void ()>>;
  inline ref_action action(std::function<void ()> a) {return std::make_shared<std::function<void ()>>(a);}
  inline void execute(ref_action a) {(*a)();}
  
  using ref_wire = std::shared_ptr<std::tuple<bool, std::list<wref_action>>>;
  auto wire() {return std::make_shared<std::tuple<bool, std::list<wref_action>>>(false, std::list<wref_action>());}
  inline bool value(ref_wire w) {return std::get<0>(*w);}
  inline void link(ref_wire w, ref_action a) {std::get<1>(*w).push_back(a);}

  inline void set(ref_wire w, bool v) {
    if(v == value(w)) return;
    std::get<0>(*w) = v;
    auto& actions = std::get<1>(*w);
    for(auto it = actions.begin(); it != actions.end(); /* nothing here */)
      if(auto a = it->lock(); a) {execute(a); ++it;}
      else                       {it = actions.erase(it);}
  }

  using ref_agenda = std::shared_ptr<std::tuple<double, std::multimap<double, std::tuple<ref_wire, bool>>>>;
  auto agenda() {return std::make_shared<std::tuple<double, std::multimap<double, std::tuple<ref_wire, bool>>>>(0, std::multimap<double, std::tuple<ref_wire, bool>>());}
  inline double now(ref_agenda a) {return std::get<0>(*a);}
  inline void notify(ref_agenda a, ref_wire w, bool status, double delay) {std::get<1>(*a).insert({now(a) + delay, {w, status}});}
  
  inline bool next(ref_agenda a) {
    auto& requests = std::get<1>(*a);
    if(requests.size() == 0) return false;

    auto next_it = requests.begin();
    auto [time, request] = *next_it;
    requests.erase(next_it);
    std::get<0>(*a) = time;
    set(std::get<0>(request), std::get<1>(request));
    return true;
  }

  inline void flush(ref_agenda a) {while(next(a));}
  inline void flush(ref_agenda a, unsigned int nb_steps) {unsigned int n = 0; while((n++ < nb_steps) && next(a));}


  inline auto PROBE(ref_agenda a, const std::string& msg, ref_wire w) {
    auto act = action([a, msg, w](){____display(now(a), msg, value(w));});
    link(w, act);
    return act;
  }

  inline auto AND(ref_agenda a, ref_wire in_1, ref_wire in_2, ref_wire out) {
    auto act = action([a, in_1, in_2, out](){notify(a, out, value(in_1) && value(in_2), .5);});
    link(in_1, act);
    link(in_2, act);
    execute(act); // Sets the effect on the gate plugging on the output wire.
    return act;
  }
  
  inline auto OR(ref_agenda a, ref_wire in_1, ref_wire in_2, ref_wire out) {
    auto act = action([a, in_1, in_2, out](){notify(a, out, value(in_1) || value(in_2), .5);});
    link(in_1, act);
    link(in_2, act);
    execute(act); // Sets the effect on the gate plugging on the output wire.
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


