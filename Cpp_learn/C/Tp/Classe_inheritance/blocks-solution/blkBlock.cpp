#include <string>
#include <iomanip>

#include "blkBlock.hpp"

std::ostream& blk::operator<<(std::ostream& os, blk::expr e) {
  if(e != nullptr) {
    int max = e->max_line();
    for(int line = e->min_line(); line < max; ++line) {
      os << std::setw(4) << line << " :   ";
      e->print_line(os, line);
      os << std::endl;
    }
  }
  else
    os << "null";
  return os;
}

int blk::Block::min_line() const {
  return -ref_height;
}

int blk::Block::max_line() const {
  return height - ref_height;
}

void blk::Block::print_line(std::ostream& os, int line) const {
  if((line >= min_line()) && (line < max_line()))
    print_inbounds_line(os, line);
  else
    os << std::string(width, ' ');
}

blk::Block::Block(int width, int height, int ref_width, int ref_height)
  : width(width), height(height), ref_width(ref_width), ref_height(ref_height) {}

blk::Block::~Block() {}

int blk::Block::get_width()      const {return width;}
int blk::Block::get_height()     const {return height;}
int blk::Block::get_ref_width()  const {return ref_width;}
int blk::Block::get_ref_height() const {return ref_height;}

