#include <cmath>
#include "lab.h"

double f(double x) {
  return std::exp(-x) - x;
}

double df(double x) {
  return -1 - std::exp(-x);
}

extern "C" TestInfo testInfo = {
  f,  "e^(-x) - x",
  df, "-1 - e^(-x)"
};
