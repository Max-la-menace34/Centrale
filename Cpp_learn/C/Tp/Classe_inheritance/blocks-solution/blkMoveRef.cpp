#include <memory>

#include "blkBlock.hpp"
#include "blkMoveRef.hpp"


blk::expr blk::move_ref(blk::expr op, int ref_width, int ref_height) {
  return std::make_shared<MoveRef>(op, ref_width, ref_height);
}

blk::MoveRef::MoveRef(blk::expr op, int ref_width, int ref_height)
  : Block(op->get_width(), op->get_height(), ref_width, ref_height), op(op) {}

blk::MoveRef::~MoveRef() {}

void blk::MoveRef::print_inbounds_line(std::ostream& os, int line) const {
  op->print_line(os, line + (get_ref_height() - op->get_ref_height()));
}
