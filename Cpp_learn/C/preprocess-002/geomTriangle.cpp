#include <geomPoint.hpp>    // We will talk about points
#include <geomSegment.hpp>  // We will talk about segments
#include <geomTriangle.hpp> // We will talk about triangles


geom::Triangle::Triangle() : A(), B(), C() {}

geom::Triangle::Triangle(const geom::Point& A,
			 const geom::Point& B,
			 const geom::Point& C)
  : A(A), B(B), C(C) {}

geom::Triangle::Triangle(const geom::Triangle& other) : A(other.A), B(other.B), C(other.C) {}

geom::Triangle& geom::Triangle::operator=(const geom::Triangle& other) {
  if(this != &other) {
    A = other.A;
    B = other.B;
    C = other.C;
  }
  return *this;
}
  
double geom::length(const geom::Triangle& triangle) {
  return length(geom::Segment(triangle.A, triangle.B))
    +    length(geom::Segment(triangle.B, triangle.C))
    +    length(geom::Segment(triangle.C, triangle.A));
}

geom::Point geom::barycenter(const geom::Triangle& triangle) {
  return (triangle.A + triangle.B + triangle.C) / 3;
}
