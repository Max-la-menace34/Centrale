#include <geom.hpp>
#include <medianator.hpp>

int main(int argc, char* argv[]) {
  geom::Point A {1, 2};
  geom::Point B {0, 1};
  geom::Point C {5, 3};

  geom::Triangle t {A, B, C};
  geom::Segment  m = mediane_by_A(t);
  double         l = geom::length(m);

  return 0;
}
