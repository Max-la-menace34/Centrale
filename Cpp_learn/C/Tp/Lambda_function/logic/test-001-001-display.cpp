#include <gate.hpp>

int main(int argc, char* argv[]) {
  gate::____(); // displays a line.
  double      time   = 1.3;
  std::string name   = "the wire";
  bool        status = true;
  gate::____display(time, name,   status);
  gate::____display(   2, "test", false );
  gate::____(); // displays a line.
  return 0;
}

/* Output:

   +-----------------+---------+-----+
   | the wire        |    1.30 | on  |
   | test            |    2.00 | off |
   +-----------------+---------+-----+
*/
