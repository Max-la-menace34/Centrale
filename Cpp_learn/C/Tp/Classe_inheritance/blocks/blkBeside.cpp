#include "blkBlock.hpp"
#include "blkBeside.hpp"

// To be done
blk::expr blk::beside(blk::expr op1, blk::expr op2) {
  return std::make_shared<blk::Beside>(op1, op2);
}

blk::Beside::Beside(blk::expr op1, blk::expr op2):
    Block(compute_width(op1,op2),compute_height(op1,op2),compute_ref_width(op1,op2),compute_ref_height(op1,op2)),
    op1(op1),
    op2(op2)
{}
blk::Beside::~Beside() {}


int blk::Beside::compute_width(expr op1, expr op2){return op1->get_width() + op2->get_width();}
int blk::Beside::compute_height(expr op1, expr op2){return std::max(op1->get_ref_height(),op2->get_ref_height())+std::max(op1->get_height()-op1->get_ref_height(),op2->get_height()-op2->get_ref_height());}
int blk::Beside::compute_ref_width(expr op1, expr op2){return op1->get_ref_width();}
int blk::Beside::compute_ref_height(expr op1, expr op2){return std::max(op1->get_ref_height(), op2->get_ref_height());}

void blk::Beside::print_inbounds_line(std::ostream& os, int line) const {
    this ->op1->print_line(os, line);
    this-> op2->print_line(os, line);
}
