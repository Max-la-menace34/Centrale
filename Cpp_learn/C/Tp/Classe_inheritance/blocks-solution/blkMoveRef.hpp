#pragma once

namespace blk {
  expr move_ref(expr op, int ref_width, int ref_height);

  class MoveRef : public Block {
  private:
    
    expr op;
    
  public:
    
    MoveRef(expr op, int ref_width, int ref_height);
    virtual ~MoveRef();
    virtual void print_inbounds_line(std::ostream& os, int line) const override;
  };
}
