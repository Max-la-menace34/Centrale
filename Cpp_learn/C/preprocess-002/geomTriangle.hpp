#pragma once

// We need to talk about points in this file.
#include <geomPoint.hpp>

namespace geom {
  struct Triangle {
    Point A;
    Point B;
    Point C;

    Triangle();
    Triangle(const Point& A, const Point& B, const Point& C);
    Triangle(const Triangle& other);
    Triangle& operator=(const Triangle& other);
  };

  double length    (const Triangle& triangle);
  Point  barycenter(const Triangle& triangle);
}
