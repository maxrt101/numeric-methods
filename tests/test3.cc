#include <cmath>
#include "lab.h"

double f(double x) {
  return std::pow(x, 2) - std::cos(x);
}

double df(double x) {
  return 2 * x + std::sin(x);
}

extern "C" TestInfo testInfo = {
  f,  "x^2 - cos(x)",
  df, "2x + sin(x)"
};
