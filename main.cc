#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <dlfcn.h>

#include "lab.h"

template <typename T>
std::vector<T> solveEquationSystem(const std::vector<std::vector<T>> A, const std::vector<T> B) {
  const int n = A.size();

  std::vector<std::vector<T>> V, C(n, std::vector<T>(n, 0));
  std::vector<T> P, Y(n), X;

  for (int i = 0; i < n; i++) {
    C[i][i] = 1;
  }

  V = A;
  P = B;

  // direct
  for (int k = 0; k < n; k++) {
    Y[k] = P[k] / V[k][k];

    for (int i = k + 1; i < n; i++) {
      P[i] = P[i] - V[i][k] * Y[k];

      for (int j = k + 1; j < n; j++) {
        C[k][j] = V[k][j] / V[k][k];
        V[i][j] = V[i][j] - V[i][k] * C[k][j];
      }
    }
  }

  X = Y;

  // reverse
  for (int i = n - 1; i >= 0; i--) {
    T sum = 0;
    for (int j = i + 1; j < n; j++) {
      sum += C[i][j] * X[j];
    }
    X[i] = Y[i] - sum;
  }

  return X;
}

std::vector<double> solveNewton(
    const std::vector<FunctionT> functions,
    const std::vector<std::vector<FunctionT>> dfunctions,
    std::vector<double> x,
    const double e) {

  const int n = functions.size();

  std::vector<std::vector<double>> E(n, std::vector<double>(n, 0)),
                                   J(n, std::vector<double>(n, 0)),
                                   invers(n, std::vector<double>(n, 0));
  std::vector<double> F(n, 0), oldX(x);

  for (int i = 0; i < n; i++) {
    E[i][i] = 1;
  }

  bool isDone = false;

  while (!isDone) {
    for (int i = 0; i < n; i++) {
      F[i] = functions[i](x);
      for (int j = 0; j < n; j++) {
        J[i][j] = dfunctions[i][j](x);
      }
    }

    for (int b = 0; b < n; b++) {
      std::vector<double> B(n);
      for (int i = 0; i < n; i++) {
        B[i] = E[i][b];
      }

      auto X = solveEquationSystem<double>(J, B);

      for (int i = 0; i < n; i++) {
        invers[i][b] = X[i];
      }
    }

    for (int i = 0; i < n; i++) {
      double sum = 0;
      for (int j = 0; j < n; j++) {
        sum += invers[i][j] * F[j];
      }
      x[i] = oldX[i] - sum;
    }

    for (int i = 0; i < n; i++) {
      if (std::abs((x[i] - oldX[i]) / x[i]) < e) isDone = true;
      oldX[i] = x[i];
    }
  }

  return x;
}

void die(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "\u001b[31mERROR\033[0m: ");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
}

void runTest(const char* filename, std::vector<double> x, double e) {
  void* handle = dlopen(filename, RTLD_NOW);
  if (!handle)
    die("Cannot open test file '%s'", filename);

  TestInfo* testInfo = (TestInfo*) dlsym(handle, TEST_INFO_VAR);
  if (!testInfo)
    die("Cannot find %s symbol in %s", TEST_INFO_VAR, filename);

  auto result = solveNewton(testInfo->functions, testInfo->dfunctions, x, e);

  for (int i = 0; i < result.size(); i++) {
    printf("x[%d] = %lf\n", i, result[i]);
  }

  dlclose(handle);
}

int main(int argc, char ** argv) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s FILE E X...\n", argv[0]);
    return 1;
  }

  double e = std::stod(argv[2]);
  std::vector<double> x;

  for (int i = 3; i < argc; i++) {
    x.push_back(std::stod(argv[i]));
  }

  runTest(argv[1], x, e);

  return 0;
}
