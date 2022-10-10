#include <iostream>
#include <gate.hpp>

int main(int argc, char* argv[]) {
  gate::ref_agenda unused_agenda = gate::agenda();
  auto             simulator     = gate::agenda();
  auto wire_a                    = gate::wire();

  auto print_wire_a              = gate::action([wire_a, simulator](){gate::____display(gate::now(simulator), "wire_a", gate::value(wire_a));});

  gate::link(wire_a, print_wire_a);

  
  gate::notify(simulator, wire_a, true,  1.5);
  gate::notify(simulator, wire_a, false, 2.0);
  gate::____();
  while(gate::next(simulator));
  gate::____();
  std::cout << std::endl;
  /* Output:

     +-----------------+---------+-----+
     | wire_a          |    1.50 | on  |
     | wire_a          |    2.00 | off |
     +-----------------+---------+-----+
  */

  gate::notify(simulator, wire_a, true,  1.5);
  gate::notify(simulator, wire_a, false, 2.0);
  gate::____();
  while(gate::next(simulator));
  gate::____();
  std::cout << std::endl;
  /* Output:

     +-----------------+---------+-----+
     | wire_a          |    3.50 | on  |
     | wire_a          |    4.00 | off |
     +-----------------+---------+-----+
  */
  
  return 0;
}
