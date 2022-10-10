#include <algorithm>
#include <sstream>

#include "blkOperators.hpp"
#include "blkOver.hpp"
#include "blkBeside.hpp"
#include "blkMoveRef.hpp"
#include "blkText.hpp"

namespace blk {
  expr from_value(double value) {
    std::ostringstream ostr;
    ostr << value;
    return text(ostr.str());
  }
}

blk::expr blk::operator+(blk::expr a, blk::expr b) {
  return beside(a, beside(text(" + "), b));
}

blk::expr blk::operator==(blk::expr a, blk::expr b) {
  return beside(a, beside(text(" = "), b));
}
		   
blk::expr blk::operator/(blk::expr a, blk::expr b) {
  auto num   = move_ref(a, a->get_width()/2, 0);
  auto denom = move_ref(b, b->get_width()/2, 0);
  int nwidth = num->get_width();
  int dwidth = denom->get_width();
  int width  = std::max(nwidth, dwidth);
  if(width > 1)
    width += 2;
  return move_ref(over(over(num, text(std::string(width, '-'))), denom),
		  width/2, num->get_height());
}


blk::expr blk::operator+(double a, blk::expr b) {
  return from_value(a) + b;
}

blk::expr blk::operator==(double a, blk::expr b) {
  return from_value(a) == b;
}

blk::expr blk::operator/(double a, blk::expr b) {
  return from_value(a) / b;
}

blk::expr blk::operator+(blk::expr a, double b) {
  return a + from_value(b);
}

blk::expr blk::operator==(blk::expr a, double b) {
  return a == from_value(b);
}

blk::expr blk::operator/(blk::expr a, double b) {
  return a / from_value(b);
}

