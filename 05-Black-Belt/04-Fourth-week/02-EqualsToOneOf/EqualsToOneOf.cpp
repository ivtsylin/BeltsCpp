//#include "optional.h"
#include "test_runner.h"
//#include "vector.h"

using namespace std;

template <typename T, typename ... Args>
bool EqualsToOneOf(const T& x, const Args& ... args) {
    return ((x == args) || ...);
}

void Test() {
  auto x = "pear";
  ASSERT(EqualsToOneOf(x, "pear"));
  ASSERT(!EqualsToOneOf(x, "apple", "apple"));
  ASSERT(EqualsToOneOf(x, "apple", "pear"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana"));
  ASSERT(EqualsToOneOf(x, "apple", "banana", "pear"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach"));
  ASSERT(EqualsToOneOf(x, "apple", "banana", "pear", "orange"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach", "orange"));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}
