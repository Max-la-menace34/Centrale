#pragma once
#include <blkBlock.hpp>
// To be done


namespace blk {
  expr over(expr op1, expr op2);

  class Over : public Block {
  private:
    
    expr op1;
    expr op2;

    // These functions make auxiliary size and ref computation for the
    // resulting block, from the two operands. Use them as arguments
    // to the conctruction of super class Block in the Beside
    // constructor.
    static int compute_width     (expr op1, expr op2);
    static int compute_height    (expr op1, expr op2);
    static int compute_ref_width (expr op1, expr op2);
    static int compute_ref_height(expr op1, expr op2);
    
  public:
    
    Over(expr op1, expr op2);
    virtual ~Over();
    virtual void print_inbounds_line(std::ostream& os, int line) const override;
  };
}
