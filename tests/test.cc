#include <cmath>
#include "lab.h"

double f(double x) {
  return std::pow(x, 3) + 3 * x - 1;
}

double df(double x) {
  return 6 * x + 3 * std::pow(x, 2);
}

extern "C" TestInfo testInfo = {
  f,  "x^3 + 3x - 1",
  df, "6x + 3x^2"
};
