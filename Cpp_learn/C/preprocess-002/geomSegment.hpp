#pragma once

// We need to talk about points in this file.
#include <geomPoint.hpp>

namespace geom {
  struct Segment {
    Point A;
    Point B;

    Segment();
    Segment(const Point& A, const Point& B);
    Segment(const Segment& other);
    Segment& operator=(const Segment& other);
  };

  double length    (const Segment& segment);
  Point  barycenter(const Segment& segment);
}
