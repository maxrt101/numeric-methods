#include <cmath>
#include "lab.h"

extern "C" TestInfo testInfo = {
  {
    [](auto x) { return - std::pow(x[0], 2) - 0.8 * std::pow(x[1], 2) - 0.1 - x[0]; },
    [](auto x) { return 0.1 - 2 * x[0] * x[1] - x[1]; }
  },
  {
    {
      [](auto x) { return -2 * x[0] - 1; },
      [](auto x) { return -1.6 * x[1]; }
    },
    {
      [](auto x) { return -2 * x[1]; },
      [](auto x) { return -2 * x[0] - 1; }
    }
  }
};
