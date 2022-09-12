#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

template <typename T>
struct Equation {
  std::vector<T> left;
  T right;
};

template <typename T>
std::vector<T> solveEquation(std::vector<Equation<T>> equationSystem) {
  const int n = equationSystem.size();

  std::vector<std::vector<T>> A, L, U;
  std::vector<T> B, X(n), Y(n);

  for (auto& equation : equationSystem) {
    A.push_back(equation.left);
    B.push_back(equation.right);
  }

  for (int i = 0; i < n; i++) {
    L.push_back(std::vector<T>(n));
    U.push_back(std::vector<T>(n));
  }

  // LU
  for (int k = 0; k < n; k++) {
    for (int i = k; i < n; i++) {
      T sum = 0;
      for (int m = 0; m < k - 1; m++) {
        sum += L[i][m] * U[m][k];
      }
      L[i][k] = A[i][k] - sum;
    }

    for (int j = k + 1; j < n; j++) {
      T sum = 0;
      for (int m = 0; m < k - 1; m++) {
        sum += L[k][m] * U[m][j];
      }
      U[k][j] = (A[k][j] - sum) / L[k][k];
    }

    U[k][k] = 1;
  }

  // direct
  Y[0] = B[0] / L[0][0];

  for (int i = 1; i < n; i++) {
    T sum = 0;
    for (int m = 0; m < i - 1; m++) {
      sum += L[i][m] * Y[m];
    }
    Y[i] = (B[i] - sum) / L[i][i];
  }

  // reverse
  // X[n-1] = Y[n-1];
  X = Y;

  for (int i = n - 1; i >= 0; i--) {
    T sum = 0;
    for (int m = i + 1; m < n; m++) {
      sum += U[i][m] * X[m];
    }
    X[i] = Y[i] - sum;
  }

  return X;
}

int main(int argc, char ** argv) {
  /*
  [8.3, 2.88, 4.1, 1.9],
  [3.92, 8.45, 7.52, 2.46],
  [3.77, 7.47, 8.04, 2.28],
  [2.21, 3.39, 1.69, 6.69]

  -11.29, 12.21, 14.81, -8.35
  */
  std::vector<Equation<double>> equation = {
    {{8.3, 2.88, 4.1, 1.9}, -11.29},
    {{3.92, 8.45, 7.52, 2.46}, 12.21},
    {{3.77, 7.47, 8.04, 2.28}, 14.81},
    {{2.21, 3.39, 1.69, 6.69}, -8.35},
  };

  auto x = solveEquation(equation);

  for (int i = 0; i < x.size(); i++) {
    printf("x%d = %lf\n", i, x[i]);
  }
}
