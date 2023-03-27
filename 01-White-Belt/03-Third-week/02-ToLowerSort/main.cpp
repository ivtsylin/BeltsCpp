#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

string tolower_str(const string& str) {
    string str2 = str;
    transform(str.begin(), str.end(), str2.begin(), ::tolower);
    return str2;
}

int main() {
    int q;
    cin >> q;
    vector<string> v;
    string tmp;
    for (int i = 0; i < q; i++) {
        cin >> tmp;
        v.push_back(tmp);
    }
    sort(v.begin(), v.end(), [] (string str1, string str2) {return tolower_str(str1) < tolower_str(str2);} );
    for (const auto &a : v) {
        cout << a << " ";
    }
    cout << endl;
    return 0;
}
