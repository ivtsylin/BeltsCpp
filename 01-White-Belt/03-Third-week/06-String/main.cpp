#include <iostream>
#include <string>

using namespace std;

class ReversibleString {
private:
    string str;
public:
    ReversibleString(const string &src_str = "") {
        str = src_str;
    }
    void Reverse() {
        string result;
        int n = str.size();
        for (int i = n - 1; i >= 0; --i) {
            result += str[i];
        }
        str = result;
    }
    string ToString() const {
        return str;
    }
};

int main() {
  ReversibleString s("live");
  s.Reverse();
  cout << s.ToString() << endl;
  
  s.Reverse();
  const ReversibleString& s_ref = s;
  string tmp = s_ref.ToString();
  cout << tmp << endl;
  
  ReversibleString empty;
  cout << '"' << empty.ToString() << '"' << endl;
  
  return 0;
}
