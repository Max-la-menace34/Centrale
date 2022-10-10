#pragma once
#include <gate.hpp>

namespace gate {
  // This defines the 1 bit adder. 
  ref_action bit_adder(ref_agenda sim,
		       ref_wire   in_1,
		       ref_wire   in_2,
		       ref_wire   in_carry,
		       ref_wire   out,
		       ref_wire   out_carry) {

    auto w1 = wire();
    auto w2 = wire();
    auto w3 = wire();

    auto g1 = XOR(sim, in_1,     in_2,        w1);
    auto g2 = XOR(sim,   w1, in_carry,       out);
    auto g3 = AND(sim,   w1, in_carry,        w2);
    auto g4 = AND(sim, in_1,     in_2,        w3);
    auto g5 = OR (sim,   w2,       w3, out_carry);

    return action([g1,g2,g3,g4,g5]() {});
  }

  // This sets up a adder. a,b,c are required to contain the same number
  // of wires.
  ref_action adder(ref_agenda             sim,
		   std::vector<ref_wire>& a,
		   std::vector<ref_wire>& b,
		   std::vector<ref_wire>& c) {
    // This stores the created gates.
    std::vector<ref_action> gates;

    auto ait = a.begin();
    auto bit = b.begin();
    auto cit = c.begin();

    auto in_carry = wire();
    auto out_carry = wire();

    // We built 1-bit adders one after the others, connection the output
    // carry of each one to the input carry of the next one.
    for(auto gate_it = std::back_inserter(gates);
	ait != a.end(); 
	++ait, ++bit, ++cit, in_carry = out_carry)
      // We create the circuit and collect the created gates.
      *(gate_it++) = bit_adder(sim, 
			       *ait, *bit, in_carry,
			       *cit, out_carry = wire());

    return action([gates]() {});
  }
							    
}
