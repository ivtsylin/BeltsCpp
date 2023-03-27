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
        Person p;
        string answer = p.GetFullName(2000);
        string expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
    }
    {
        Person p;
        string answer, expected_answer;
        
        p.ChangeFirstName(2000, "Ivan");
        
        answer = p.GetFullName(1999);
        expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2000);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2001);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
    }
    {
        Person p;
        string answer, expected_answer;
        
        p.ChangeLastName(2000, "Petrov");
        
        answer = p.GetFullName(1999);
        expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2000);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2001);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
    }
    {
        Person p;
        string answer, expected_answer;
        
        p.ChangeFirstName(1995, "Ivan");
        p.ChangeLastName(2000, "Petrov");
        
        answer = p.GetFullName(1994);
        expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
        
        
        answer = p.GetFullName(1995);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1999);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2000);
        expected_answer = "Ivan Petrov";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2001);
        expected_answer = "Ivan Petrov";
        AssertEqual(answer, expected_answer);
    }
    {
        Person p;
        string answer, expected_answer;
        
        p.ChangeFirstName(2000, "Ivan");
        p.ChangeLastName(1995, "Petrov");
        
        answer = p.GetFullName(1994);
        expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
        
        
        answer = p.GetFullName(1995);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1999);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2000);
        expected_answer = "Ivan Petrov";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2001);
        expected_answer = "Ivan Petrov";
        AssertEqual(answer, expected_answer);
    }
    {
        Person p;
        string answer, expected_answer;
        
        p.ChangeFirstName(1995, "Ivan");
        p.ChangeFirstName(2000, "Vanya");
        p.ChangeFirstName(2005, "Ivan");
        
        answer = p.GetFullName(1994);
        expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1995);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1996);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1999);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2000);
        expected_answer = "Vanya with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2001);
        expected_answer = "Vanya with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2004);
        expected_answer = "Vanya with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2005);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2006);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
    }
    {
        Person p;
        string answer, expected_answer;
        
        p.ChangeLastName(1995, "Petrov");
        p.ChangeLastName(2000, "Sidorov");
        p.ChangeLastName(2005, "Petrov");
        
        answer = p.GetFullName(1994);
        expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1995);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1996);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1999);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2000);
        expected_answer = "Sidorov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2001);
        expected_answer = "Sidorov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2004);
        expected_answer = "Sidorov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2005);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2006);
        expected_answer = "Petrov with unknown first name";
        AssertEqual(answer, expected_answer);
    }
    {
        Person p;
        string answer, expected_answer;
        
        p.ChangeFirstName(1995, "Ivan");
        p.ChangeLastName(1996, "Petrov");
        p.ChangeFirstName(1997, "Vanya");
        p.ChangeLastName(1998, "Sidorov");
        p.ChangeFirstName(1999, "Ivan");
        p.ChangeLastName(2000, "Petrov");
        
        answer = p.GetFullName(1994);
        expected_answer = "Incognito";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1995);
        expected_answer = "Ivan with unknown last name";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1996);
        expected_answer = "Ivan Petrov";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1997);
        expected_answer = "Vanya Petrov";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1998);
        expected_answer = "Vanya Sidorov";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(1999);
        expected_answer = "Ivan Sidorov";
        AssertEqual(answer, expected_answer);
        
        answer = p.GetFullName(2000);
        expected_answer = "Ivan Petrov";
        AssertEqual(answer, expected_answer);
   
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(UnitTests, "UnitTests");
    return 0;
}
