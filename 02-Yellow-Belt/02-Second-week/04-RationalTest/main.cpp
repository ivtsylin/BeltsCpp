#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

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

void UnitTests() {
    {
        Rational r;
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 0;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(0, 1);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 0;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(0, -1);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 0;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(0, 12);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 0;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(0, -12);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 0;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(1, 1);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(11, 11);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(-1, -1);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(-11, -11);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(1, -1);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = -1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(-1, 1);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = -1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(-100, 100);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = -1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(100, -100);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = -1;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 1;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(50, 20);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 5;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 2;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(20, 50);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 2;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 5;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(-20, -50);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = 2;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 5;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(20, -50);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = -2;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 5;
        AssertEqual(denom, exp_denom);
    }
    {
        Rational r(-20, 50);
        double num, denom, exp_num, exp_denom;
        
        num = r.Numerator();
        exp_num = -2;
        AssertEqual(num, exp_num);
        
        denom = r.Denominator();
        exp_denom = 5;
        AssertEqual(denom, exp_denom);
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(UnitTests, "UnitTests");
    return 0;
}
