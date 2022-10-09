#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using FunctionT = double(*)(std::vector<double>, double); // x, t

const double U_MAX = 100;
const double F = 50;
const double R1 = 5;
const double R2 = 4;
const double R3 = 7;
const double R4 = 2;
const double C1 = 300 * std::pow(10, -6);
const double C2 = 150 * std::pow(10, -6);
const double C3 = 200 * std::pow(10, -6);

const auto U1 = [](double t) { return U_MAX * std::sin(2 * M_PI * F * t); };


std::vector<double> eulerMethod(std::vector<FunctionT> functions, double t1, double t2, double h) {
  const int n = functions.size();
  std::vector<double> x(n, 0), result;

  for (double t = t1; t < t2; t += h) {
    std::vector<double> prev;
    for (int i = 0; i < n; i++) {
      prev.push_back(x[i] + h * functions[i](x, t));
    }
    for (int i = 0; i < n; i++) {
      x[i] += h * functions[i](prev, t);
    }
    result.push_back(x[n-1]);
  }

  return result;
}

void plot(const std::string& file, const std::vector<double>& result, double t2, double h) {
  std::ofstream resfile("result.txt");

  for (int i = 0; i < result.size(); i++) {
    resfile << result[i] << " ";
  }

  resfile.close();

  char* command = new char[1024];

  snprintf(command, 1024,
    "python3.10 -c \""
      "import matplotlib.pylab as pl;"
      "import numpy as np;"
      "pl.plot(np.arange(0, %lf, %lf), [float(x) for x in filter(None, open('result.txt').read().split(' '))]);"
      "pl.ylabel('U2');"
      "pl.xlabel('Current Time');"
      "pl.savefig('%s');"
    "\"", t2, h, file.c_str()
  );

  system(command);

  delete [] command;
}

int main(int argc, char ** argv) {
  double t1 = 0, t2 = 0.2, h = 0.00001;

  if (argc > 1) t1 = std::stod(argv[1]);
  if (argc > 2) t2 = std::stod(argv[2]);
  if (argc > 3) h  = std::stod(argv[3]);

  auto result = eulerMethod(
    {
      [](std::vector<double> x, double t) {
        return ((U1(t) - x[1] - x[0] * R2 - x[0] * R3 + (x[0] * R2) - x[2]) / (R1 + R3)) / C1;
      },
      [](std::vector<double> x, double t) {
        return x[0] / C2;
      },
      [](std::vector<double> x, double t) {
        return (((U1(t) - x[1] - x[0] * R2 - x[0] * R3 + (x[0] * R2) - x[2]) / (R1 + R3)) - x[0]) / C3;
      }
    },
    t1, t2, h
  );

  plot("graph.png", result, t2, h);

  return 0;
}
