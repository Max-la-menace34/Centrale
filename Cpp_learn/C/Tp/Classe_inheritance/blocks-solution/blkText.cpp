#include <string>
#include <memory>

#include <blkText.hpp>

blk::expr blk::text(const std::string& label, align alignment) {
  return std::make_shared<Text>(label, alignment);
}

int blk::Text::compute_ref_width(const std::string& label, align alignment) {
  switch(alignment) {
  case align::left  : return                0; break;
  case align::center: return label.size() / 2; break;
  case align::right : return label.size() - 1; break;
  }

  return 0; // Avoids a warning... this code is never reached.
}

blk::Text::Text(const std::string& label, align alignment)
  : Block(label.size(), 1, compute_ref_width(label, alignment), 0),
    label(label) {}

blk::Text::~Text() {}

void blk::Text::print_inbounds_line(std::ostream& os, int line) const {
  os << label;
}
