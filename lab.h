#ifndef _LAB_H_
#define _LAB_H_ 1

#include <functional>
#include <vector>

#define TEST_INFO_VAR "testInfo"

using FunctionT = double(*)(std::vector<double>);

struct TestInfo {
  std::vector<FunctionT> functions;
  std::vector<std::vector<FunctionT>> dfunctions;
};

#endif /* _LAB_H_ */
