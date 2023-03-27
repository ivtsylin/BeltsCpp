#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int GetDistinctRealRootCount(double a, double b, double c);

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
       os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

void TestLinear() {
    {
        double a = 0, b = 1, c = 0;
        int result = 1;
        AssertEqual(result, GetDistinctRealRootCount(a, b, c));
    }
    {
        double a = 0, b = 1, c = 1;
        int result = 1;
        AssertEqual(result, GetDistinctRealRootCount(a, b, c));
    }
    {
        double a = 0, b = 0, c = 1;
        int result = 0;
        AssertEqual(result, GetDistinctRealRootCount(a, b, c));
    }
}

void TestSquare() {
    {
        double a = 1, b = 0, c = 0;
        int result = 1;
        AssertEqual(result, GetDistinctRealRootCount(a, b, c));
    }
    {
        double a = 1, b = 2, c = 1;
        int result = 1;
        AssertEqual(result, GetDistinctRealRootCount(a, b, c));
    }
    {
        double a = 1, b = 0, c = -1;
        int result = 2;
        AssertEqual(result, GetDistinctRealRootCount(a, b, c));
    }
    {
        double a = 1, b = 0, c = 1;
        int result = 0;
        AssertEqual(result, GetDistinctRealRootCount(a, b, c));
    }
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

int GetDistinctRealRootCount(double a, double b, double c) {
    if (a == 0) {
        if (b == 0) {
            return 0;
        }
        return 1;
    }
    double dis = b * b - 4 * a * c;
    if (fabs(dis) < std::numeric_limits<double>::epsilon()) {
        return 1;
    } else if (dis > 0) {
        return 2;
    }
    return 0;
}

int main() {
    TestRunner runner;
    runner.RunTest(TestLinear, "TestLinear");
    runner.RunTest(TestSquare, "TestSquare");
    return 0;
}
