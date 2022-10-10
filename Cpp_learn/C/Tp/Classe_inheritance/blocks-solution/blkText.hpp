#pragma once

#include <string>
#include <blkBlock.hpp>

namespace blk {

  enum class align : char {left, center, right};
  
  expr text(const std::string& label, align alignment = align::center);

  class Text : public Block {
  private:

    std::string label;

    static int compute_ref_width(const std::string& label, align alignment);
    
  public:

    Text(const std::string& label, align alignment);
    virtual ~Text();
    virtual void print_inbounds_line(std::ostream& os, int line) const override;
  };

}

