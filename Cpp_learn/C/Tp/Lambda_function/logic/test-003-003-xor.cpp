#include <gate.hpp>

int main(int argc, char* argv[]) {
  auto sim = gate::agenda();

  auto a   = gate::wire();
  auto b   = gate::wire();
  auto c   = gate::wire();
  
  auto pa  = gate::PROBE(sim, "a", a);
  auto pb  = gate::PROBE(sim, "b", b);
  auto pc  = gate::PROBE(sim, "c", c);

  auto g   = gate::XOR(sim, a, b, c); 

  gate::notify(sim, a, true,  10.0);
  gate::notify(sim, b, true,  15.0);
  gate::notify(sim, b, false, 20.0);
  gate::notify(sim, b, true,  20.5);

  gate::____();
  gate::flush(sim);
  gate::____();
  
  return 0;
}

/* Outut:

   +-----------------+---------+-----+
   | a               |   10.00 | on  |
   | c               |   11.00 | on  |
   | b               |   15.00 | on  |
   | c               |   16.10 | off |
   | b               |   20.00 | off |
   | b               |   20.50 | on  |
   | c               |   21.10 | on  |
   | c               |   21.60 | off |
   +-----------------+---------+-----+
*/
