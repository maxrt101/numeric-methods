#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>


template <typename T>
struct Equation {
  std::vector<T> left;
  T right;
};

template <typename T>
T stringToT(const std::string& s) { return T{}; }

template <> int stringToT<int>(const std::string& s) { return std::stoi(s); }
template <> long stringToT<long>(const std::string& s) { return std::stol(s); }
template <> float stringToT<float>(const std::string& s) { return std::stof(s); }
template <> double stringToT<double>(const std::string& s) { return std::stod(s); }

std::vector<std::string> split(const std::string& str, const std::string& delimiter = " ") {
  std::vector<std::string> result;
  size_t last = 0, next = 0;
  while ((next = str.find(delimiter, last)) != std::string::npos) {
    result.push_back(str.substr(last, next-last));
    last = next + 1;
  }
  result.push_back(str.substr(last));
  return result;
}

template <typename T>
void printEquationSystem(const std::vector<Equation<T>>& equation) {
  for (int i = 0; i < equation.size(); i++) {
    for (int j = 0; j < equation[i].left.size(); j++) {
      printf("%.02lf ", equation[i].left[j]);
    }
    printf("= %.02lf\n", equation[i].right);
  }
}

template <typename T>
std::vector<T> solveEquation(const std::vector<Equation<T>> equationSystem) {
  const int n = equationSystem.size();

  std::vector<std::vector<T>> A, V, C(n, std::vector<T>(n, 0));
  std::vector<T> B, P, Y(n), X;

  for (auto& equation : equationSystem) {
    A.push_back(equation.left);
    B.push_back(equation.right);
  }

  for (int i = 0; i < n; i++) {
    C[i][i] = 1;
  }

  V = A;
  P = B;

  // direct
  for (int k = 0; k < n; k++) {
    // sort
    T max = std::abs(V[k][k]);
    int w = k;

    for (int l = k + 1; l < n; l++) {
      if (max < std::abs(V[l][k])) {
        max = std::abs(V[l][k]);
        w = l;
      }
    }

    T tmp = P[k];
    P[k] = P[w];
    P[w] = tmp;

    for (int d = 0; d < n; d++) {
      tmp = V[k][d];
      V[k][d] = V[w][d];
      V[w][d] = tmp;
    }

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

template <typename T>
std::vector<Equation<T>> scanEquationSystem(const std::string& src) {
  std::vector<Equation<T>> result;

  std::vector<std::string> lines = split(src, "\n");

  for (int i = 0; i < lines.size(); i++) {
    if (lines[i].empty()) continue;

    std::vector<std::string> tokens = split(lines[i], " ");
    Equation<T> equation;

    int j = 0;
    while (tokens[j] != "=") {
      equation.left.push_back(stringToT<T>(tokens[j++]));
    }

    if (j+1 < tokens.size()) {
      equation.right = stringToT<T>(tokens[j+1]);
    }

    result.push_back(equation);
  }

  return result;
}

// std::vector<Equation<double>> equation = {
//   {{8.3,  2.98, 4.1,  1.9}, -10.01},
//   {{3.92, 8.45, 7.42, 2.46}, 12.21},
//   {{3.77, 7.57, 8.04, 2.28}, 14.81},
//   {{2.21, 3.29, 1.69, 6.69}, -8.35}
// };

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    return 1;
  }

  std::ifstream file(argv[1]);
  std::string str((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());

  auto equation = scanEquationSystem<double>(str);

  printEquationSystem(equation);

  auto x = solveEquation(equation);

  for (int i = 0; i < x.size(); i++) {
    printf("x%d = %lf\n", i, x[i]);
  }

  return 0;
}
