#include <iostream>
#include <gate.hpp>


int main(int argc, char* argv[]) {
  auto sim = gate::agenda();

  auto wire_a = gate::wire();
  auto wire_b = gate::wire();

  gate::set(wire_a, false);
  gate::set(wire_b, true );

  auto probe_a = gate::action([wire_a, sim](){gate::____display(gate::now(sim), "a", gate::value(wire_a));});
  gate::link(wire_a, probe_a);
  
  auto probe_b = gate::action([wire_b, sim](){gate::____display(gate::now(sim), "b", gate::value(wire_b));});
  gate::link(wire_b, probe_b);

#define NOT_DURATION 1.25
  
  auto not_gate_1 = gate::action([wire_a, wire_b, sim](){gate::notify(sim, wire_b, !gate::value(wire_a), NOT_DURATION);});
  gate::link(wire_a, not_gate_1);
  
  auto id_gate_2  = gate::action([wire_b, wire_a, sim](){gate::notify(sim, wire_a, gate::value(wire_b), NOT_DURATION);});
  gate::link(wire_b, id_gate_2);
  
  gate::____();
  gate::set(wire_a, true);
  gate::____();
  gate::flush(sim, 10); // Makes at most 10 simulation steps.
  // gate::flush(sim);  // Runs until no more requests are pending (will loop here).
  gate::____();
  
  return 0;
}

/*
  Output:

  +-----------------+---------+-----+
  | a               |    0.00 | on  |
  +-----------------+---------+-----+
  | b               |    1.25 | off |
  | a               |    2.50 | off |
  | b               |    3.75 | on  |
  | a               |    5.00 | on  |
  | b               |    6.25 | off |
  | a               |    7.50 | off |
  | b               |    8.75 | on  |
  | a               |   10.00 | on  |
  | b               |   11.25 | off |
  | a               |   12.50 | off |
  +-----------------+---------+-----+
*/
