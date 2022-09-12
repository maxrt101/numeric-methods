#include <cmath>
#include "lab.h"

double f(double x) {
  return std::pow(std::sin(x), 3) * std::cos(x);
}

double F(double x) {
  return std::pow(std::sin(x), 4) / 4;
}

extern "C" TestInfo testInfo = {
  f, F,
  0, M_PI/2
};
