#include <gate.hpp>
#include <adder.hpp>
#include <ranges>

int main(int agrc, char* argv[]) {

  auto sim = gate::agenda();

  std::vector<gate::ref_wire> a(8);
  std::vector<gate::ref_wire> b(8);
  std::vector<gate::ref_wire> c(8);

  // We make the wires, and the probes for the outputs.
  for(auto& w : a) w = gate::wire();
  for(auto& w : b) w = gate::wire();
  for(auto& w : c) w = gate::wire();

  auto gates = gate::adder(sim, a, b, c);

  gate::set(a[0],  true); // 1
  gate::set(a[1],  true); // 1
  gate::set(a[2], false); // 0
  gate::set(a[3],  true); // 1
  gate::set(a[4], false); // 0

  gate::set(b[0],  true); // 1
  gate::set(b[1], false); // 0
  gate::set(b[2],  true); // 1
  gate::set(b[3],  true); // 1
  gate::set(b[4], false); // 0
  
  gate::flush(sim);

  std::cout << "  a:  ";
  for(auto w : a | std::views::reverse) std::cout << (int)(gate::value(w));
  std::cout << std::endl;
  
  std::cout << "+ b:  ";
  for(auto w : b | std::views::reverse) std::cout << (int)(gate::value(w));
  std::cout << std::endl;

  std::cout << "     ----------" << std::endl
	    << "= c:  ";
  for(auto w : c | std::views::reverse) std::cout << (int)(gate::value(w));
  std::cout << std::endl;

  return 0;
}


