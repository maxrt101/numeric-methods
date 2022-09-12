#include <cmath>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <dlfcn.h>

#include "lab.h"

void die(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "\u001b[31mERROR\033[0m: ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
}

double secantMethod(FunctionT f, FunctionT df, double x0, double x1, double e) {
  double olderX = 0, oldX = x0, x = x1;

  while (1) {
    olderX = oldX;
    oldX = x;

    double fOlderX = f(olderX), fX = f(x);

    x -= (fX / ((fOlderX - fX) / (olderX - x)));

    if (std::abs((x - oldX) / x) < e) break;
  }

  return x;
}

void runTest(const char* filename, double x0, double x1, double e) {
  void* handle = dlopen(filename, RTLD_NOW);
  if (!handle)
    die("Cannot open test file '%s'", filename);

  TestInfo* testInfo = (TestInfo*) dlsym(handle, TEST_INFO_VAR);
  if (!testInfo)
    die("Cannot find %s symbol in %s", TEST_INFO_VAR, filename);

  double x = secantMethod(
    testInfo->f,
    testInfo->df,
    x0, x1, e
  );

  printf("f=%s\ndf=%s\nx=%lf\nf(x)=%e\n", testInfo->fText, testInfo->dfText, x, testInfo->f(x));

  dlclose(handle);
}


int main(int argc, char ** argv) {
  if (argc != 5) {
    fprintf(stderr, "Usage: %s FILE X0 X1 E\n", argv[0]);
    return 1;
  }

  runTest(argv[1], std::stod(argv[2]), std::stod(argv[3]), std::stod(argv[4]));

  return 0;
}
