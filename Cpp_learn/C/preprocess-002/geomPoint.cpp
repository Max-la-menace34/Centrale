#include <cmath>
#include <geomPoint.hpp>


geom::Point::Point() : x(0), y(0) {}

geom::Point::Point(double x, double y) : x(x), y(y) {}

geom::Point::Point(const geom::Point& other) : x(other.x), y(other.y) {}

geom::Point& geom::Point::operator=(const geom::Point& other) {
  if(this != &other) {
    x = other.x;
    y = other.y;
  }
  return *this;
}

geom::Point geom::Point::operator+(const geom::Point& other) const {
  return {x + other.x, y + other.y};
}

geom::Point geom::Point::operator-(const geom::Point& other) const {
  return {x - other.x, y - other.y};
}

double geom::Point::operator*(const geom::Point& other) const {
  return x * other.x + y * other.y;
}

double geom::Point::norm() const {
  return std::sqrt((*this) * (*this));
}
