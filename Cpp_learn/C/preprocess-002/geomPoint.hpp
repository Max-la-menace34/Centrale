#pragma once

namespace geom {
  struct Point {
    double x;
    double y;

    Point();
    Point(double x, double y);
    Point(const Point& other);
    Point& operator=(const Point& other);

    Point  operator+(const Point& other) const;
    Point  operator-(const Point& other) const;
    double operator*(const Point& other) const;
    double norm()                        const;
  };

  // These are function definitions right here, not only headers. We do
  // this for external operators... this will trigger some errors.
  inline Point operator*(double a, const Point& p) {return {a * p.x, a * p.y};}
  inline Point operator*(const Point& p, double a) {return a * p;}
  inline Point operator/(const Point& p, double a) {return {p.x / a, p.y / a};}
}

