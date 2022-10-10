#include "blkBlock.hpp"

// To be done
blk::Block::Block(int width,int height, int ref_width,int ref_height):
    width(width),
    height(height),
    ref_width(ref_width),
    ref_height(ref_height){}
blk::Block::~Block() {}
void blk::Block::print_line(std::ostream& os, int line) const{
if(line>=min_line() && line <max_line()){
    print_inbounds_line(os,line);
}else{
    std::string space="";
    for(int i =0;i!= width;++i){
        space += " ";
    }
    os<<space;
}
}

int blk::Block::Block::min_line() const {
    return 0-ref_height;
}
int blk::Block::Block::max_line() const {

    return height-ref_height;
}
int blk::Block::Block::get_height() const {return height;}
int blk::Block::Block::get_width() const{return width;}
int blk::Block::Block::get_ref_height() const {return ref_height;}
int blk::Block::Block::get_ref_width() const { return ref_width;}

std::ostream& blk::operator<<(std::ostream& os, blk::expr block){
    int max_line = block->max_line();
    int min_line = block ->min_line();
      if(block != nullptr) {
    for(int i = min_line; i < max_line; ++i) {
        if (i>=10){
        os << "  "<< i << " :   "<<std::flush;
        block->print_line(os, i);
        os << std::endl;

        }else if (i>=0){
        os << "   "<< i << " :   "<<std::flush;
        block->print_line(os, i);
        os << std::endl;
        }else{
        os << "  "<< i << " :   "<<std::flush;
        block->print_line(os, i);
        os << std::endl;
        }

    }
  }
  else
    os << "null";
  return os;

}