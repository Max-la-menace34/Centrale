#include "blkBlock.hpp"
#include "blkDebug.hpp"

// To be done
blk::expr blk::debug(char label, int width, int height, int ref_width, int ref_height) {
  return std::make_shared<Debug>(label, width, height, ref_width, ref_height);
}

blk::Debug::Debug(char label, int width, int height, int ref_width, int ref_height):
blk::Block(width,height,ref_width,ref_height),
label(label)
{}
blk::Debug::~Debug() {}
void blk::Debug::print_inbounds_line(std::ostream& os, int line)const {
    if(line!=0){

    os << std::string(get_width(), label);}
  else{
    for(int column = 0; column < get_width(); ++column)
      if(column == get_ref_width()){
	os << '#';}
      else{
	os << label;}}
}