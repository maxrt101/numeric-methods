#ifndef _LAB_H_
#define _LAB_H_ 1

#include <functional>

#define TEST_INFO_VAR "testInfo"

using FunctionT = double(*)(double);

struct TestInfo {
  FunctionT f;
  const char* fText;
  FunctionT df;
  const char* dfText;
};

#endif /* _LAB_H_ */
