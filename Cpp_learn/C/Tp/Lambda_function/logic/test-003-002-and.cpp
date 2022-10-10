#include <gate.hpp>

int main(int argc, char* argv[]) {
  auto agenda = gate::agenda();

  auto a      = gate::wire();
  auto b      = gate::wire();
  auto c      = gate::wire();

  gate::set(c, true); // This is for checking your AND implementation later
  //                     (see the comment when teh desired output is given).

  auto pa     = gate::PROBE(agenda, "a", a);
  auto pb     = gate::PROBE(agenda, "b", b);
  auto pc     = gate::PROBE(agenda, "c", c);

  auto g      = gate::AND(agenda, a, b, c); // Duration is .5

  gate::notify(agenda, a, true,  2.0);
  gate::notify(agenda, b, true,  4.0);
  gate::notify(agenda, b, false, 6.0);
  gate::notify(agenda, a, false, 8.0);
  gate::____();
  gate::flush(agenda);
  gate::____();
  
  return 0;
}

/* Output:

   +-----------------+---------+-----+
   | c               |    0.50 | off |  <-- be sure to have this !
   | a               |    2.00 | on  |
   | b               |    4.00 | on  |
   | c               |    4.50 | on  |
   | b               |    6.00 | off |
   | c               |    6.50 | off |
   | a               |    8.00 | off |
   +-----------------+---------+-----+
*/
