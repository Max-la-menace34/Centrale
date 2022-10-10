#include <gate.hpp>


// Here, we want to check that setting a value to a wire is only
// efficient if the value is a real change. Every display is 'on',
// whatever the status of the wire.

int main(int argc, char* argv[]) {

  gate::ref_wire unused_wire = gate::wire(); // wire is a function that we call to build 
  auto           wire_a      = gate::wire(); // a ref_wire on a (false, empty-list) wire.

  if(gate::value(wire_a)) std::cout << "Wire is ON."  << std::endl;
  else                    std::cout << "Wire is OFF." << std::endl;
  /* Output: Wire is OFF. */
  
  auto gate_1 = gate::action([](){gate::____display(1, "gate 1", true);});
  auto gate_2 = gate::action([](){gate::____display(1, "gate 2", true);});
  auto gate_3 = gate::action([](){gate::____display(1, "gate 3", true);});

  {
    auto gate_4 = gate::action([](){gate::____display(1, "gate 4", true);});
    auto gate_5 = gate::action([](){gate::____display(1, "gate 5", true);});

    gate::link(wire_a, gate_1);
    gate::link(wire_a, gate_2);
    gate::link(wire_a, gate_3);
    gate::link(wire_a, gate_4);
    gate::link(wire_a, gate_5);

    gate::____();
    gate::set(wire_a, true);
    gate::____();
    std::cout << std::endl;

    /* Output :

       +-----------------+---------+-----+
       | gate 1          |    1.00 | on  |
       | gate 2          |    1.00 | on  |
       | gate 3          |    1.00 | on  |
       | gate 4          |    1.00 | on  |
       | gate 5          |    1.00 | on  |
       +-----------------+---------+-----+
    */

    gate::____();
    gate::set(wire_a, true);
    gate::____();
    std::cout << std::endl;

    /* Output :

       +-----------------+---------+-----+
       +-----------------+---------+-----+
    */

    gate::____();
    gate::set(wire_a, false);
    gate::____();
    std::cout << std::endl;

    /* Output :

       +-----------------+---------+-----+
       | gate 1          |    1.00 | on  |
       | gate 2          |    1.00 | on  |
       | gate 3          |    1.00 | on  |
       | gate 4          |    1.00 | on  |
       | gate 5          |    1.00 | on  |
       +-----------------+---------+-----+
    */
  }

  gate::____();
  gate::set(wire_a, false);
  gate::____();
  
  /* Output :

     +-----------------+---------+-----+
     +-----------------+---------+-----+
  */
  
  gate::____();
  gate::set(wire_a, true);
  gate::____();
    
  /* Output :

     +-----------------+---------+-----+
     | gate 1          |    1.00 | on  |
     | gate 2          |    1.00 | on  |
     | gate 3          |    1.00 | on  |
     +-----------------+---------+-----+
  */
  
  return 0;
}
