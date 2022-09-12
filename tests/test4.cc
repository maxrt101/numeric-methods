#include <cmath>
#include "lab.h"

double f(double x) {
  return std::pow(x, 3) + x + 3;
}

double df(double x) {
  return 1 + 3 * std::pow(x, 2);
}

extern "C" TestInfo testInfo = {
  f,  "x^3 + x + 3",
  df, "1 + 3x^2"
};
