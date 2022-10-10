#include <gate.hpp>

int main(int argc, char* argv[]) {
  auto sim   = gate::agenda();
  auto clk   = gate::wire();
  
  auto clock = gate::action([sim, clk](){gate::notify(sim, clk, !gate::value(clk), .25);});
  gate::link(clk, clock);
  
  auto probe = gate::PROBE(sim, "clk", clk);

  gate::____();
  gate::set(clk, true);
  gate::____();
  gate::flush(sim, 10);
  gate::____();

  return 0;
}

/* Outut:

   +-----------------+---------+-----+
   | clk             |    0.00 | on  |
   +-----------------+---------+-----+
   | clk             |    0.25 | off |
   | clk             |    0.50 | on  |
   | clk             |    0.75 | off |
   | clk             |    1.00 | on  |
   | clk             |    1.25 | off |
   | clk             |    1.50 | on  |
   | clk             |    1.75 | off |
   | clk             |    2.00 | on  |
   | clk             |    2.25 | off |
   | clk             |    2.50 | on  |
   +-----------------+---------+-----+
*/
