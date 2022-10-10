#include <gate.hpp>
#include <vector>

int main(int argc, char* argv[]) {
  std::vector<gate::ref_action> actions = {gate::action([](){gate::____display(1, "A", true );}),
					   gate::action([](){gate::____display(2, "B", false);}),
					   gate::action([](){gate::____display(3, "C", true );}),
					   gate::action([](){gate::____display(4, "D", false);})};
  gate::____();
  for(auto ref_a : actions) gate::execute(ref_a);
  gate::____();
  
  return 0;
}

/* Output:
   
   +-----------------+---------+-----+
   | A               |    1.00 | on  |
   | B               |    2.00 | off |
   | C               |    3.00 | on  |
   | D               |    4.00 | off |
   +-----------------+---------+-----+
*/
