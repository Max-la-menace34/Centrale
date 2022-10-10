#include "medianator.hpp"

// This "tool" function is not exposed in any .hpp.
geom::Segment mediane(const geom::Point& H, const geom::Point& A, const geom::Point& B) {
  return {H, (A + B) / 2};
}


geom::Segment mediane_by_A(const geom::Triangle& t) {return mediane(t.A, t.B, t.C);}
geom::Segment mediane_by_B(const geom::Triangle& t) {return mediane(t.B, t.C, t.A);}
geom::Segment mediane_by_C(const geom::Triangle& t) {return mediane(t.C, t.A, t.B);}

