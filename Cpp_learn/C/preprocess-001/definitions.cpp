
#pragma once
#ifdef HIGH_PRECISION

#define PI 3.141592653589793
#define E  2.718281828459045

#else

#define PI 3.14
#define E 2.718

#endif

#define TwoPI 2*PI

struct Complex {
  double re, im;
};
