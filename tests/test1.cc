#include <cmath>
#include "lab.h"

extern "C" TestInfo testInfo = {
  {
    [](auto x) { return 4 * std::pow(x[0], 2) + std::pow(x[1], 2) - 4; },
    [](auto x) { return x[0] - std::pow(x[1], 2); }
  },
  {
    {
      [](auto x) { return 8 * x[0]; },
      [](auto x) { return 2 * x[1]; }
    },
    {
      [](auto x) { return 1.0; },
      [](auto x) { return -2 * x[1]; }
    }
  }
};
