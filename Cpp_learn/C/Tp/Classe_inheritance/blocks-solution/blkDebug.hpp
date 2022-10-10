#pragma once

namespace blk {
  // This is a factory that builds a debug block.
  expr debug(char label, int width, int height, int ref_width, int ref_height);

  class Debug : public Block {
  private:
    
    char label;
    
  public:

    Debug(char label, int width, int height, int ref_width, int ref_height);
    virtual ~Debug();

    // This one has to be implemented.
    virtual void print_inbounds_line(std::ostream& os, int line) const override;

  };
}
