#include <geomPoint.hpp>   // We will talk about points
#include <geomSegment.hpp> // This makes the previous include useless... but we do not care thanks to #pragma once.


geom::Segment::Segment() : A(), B() {}

geom::Segment::Segment(const geom::Point& A, const geom::Point& B) : A(A), B(B) {}

geom::Segment::Segment(const geom::Segment& other) : A(other.A), B(other.B) {}

geom::Segment& geom::Segment::operator=(const geom::Segment& other) {
  if(this != &other) {
    A = other.A;
    B = other.B;
  }
  return *this;
}
  
double geom::length(const geom::Segment& segment) {
  return (segment.A - segment.B).norm();
}

geom::Point geom::barycenter(const geom::Segment& segment) {
  return (segment.A + segment.B) * .5;
}
