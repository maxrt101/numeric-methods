#include <cmath>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <dlfcn.h>

#include "lab.h"

#define N 30

void die(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "\u001b[31mERROR\033[0m: ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
}

double trapezeMethod(FunctionT f, double a, double b, unsigned n) {
  double integral = 0;
  double h = (b - a) / n;
  double x = a + h;
  double fa = f(a), fb = f(b);

  for (int i = 0; i < n - 1; i++) {
    integral += f(x);
    x += h;
  }

  return h * (((fa + fb) / 2) + integral);
}

void runTest(const char* filename, double n) {
  void* handle = dlopen(filename, RTLD_NOW);
  if (!handle)
    die("Cannot open test file '%s'", filename);

  TestInfo* testInfo = (TestInfo*) dlsym(handle, TEST_INFO_VAR);
  if (!testInfo)
    die("Cannot find %s symbol in %s", TEST_INFO_VAR, filename);

  double integral = trapezeMethod(testInfo->f, testInfo->a, testInfo->b, n);

  printf("I=%e\n", integral);
  printf("NL=%e\n", testInfo->F(testInfo->b) - testInfo->F(testInfo->a));

  dlclose(handle);
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s FILE [N=30]\n", argv[0]);
    return 1;
  }

  runTest(
    argv[1],
    argc == 3 ? std::stod(argv[2]) : N
  );

  return 0;
}
