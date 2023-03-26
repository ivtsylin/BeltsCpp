#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool IsPalindrom (const string &str) {
    string rev_str = "";
    for (int i = str.length() - 1; i >= 0; --i) {
        rev_str += str [ i ];
    }
    if (str == rev_str){
        return true;
    }
    return false;
}

int main() {
    string str;
    cin >> str;
    cout << IsPalindrom (str);
    return 0;
}
