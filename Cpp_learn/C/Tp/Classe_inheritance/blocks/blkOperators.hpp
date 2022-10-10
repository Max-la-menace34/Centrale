#pragma once

#include <blkBlock.hpp>

namespace blk {
  expr operator+ (expr a, expr b);
  expr operator==(expr a, expr b);
  expr operator/ (expr a, expr b);
  
  expr operator+ (double a, expr b);
  expr operator==(double a, expr b);
  expr operator/ (double a, expr b);
  
  expr operator+ (expr a, double b);
  expr operator==(expr a, double b);
  expr operator/ (expr a, double b);
  
}
