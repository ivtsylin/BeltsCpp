#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main () {
    vector<string> ops;
    string ans;
    cin >> ans;
    ops.push_back(ans);
    int N;
    cin >> N;
    string op, rhs;
    ops.resize(1 + N);
    for (int i = 0; i < N; ++i) {
        cin >> op >> rhs;
        ops [ i+ 1 ] = op + " " + rhs;
    }
    string left_brackets (N, '(');
    cout << left_brackets << ops [ 0 ];
    for (int i = 0; i < N; ++i) {
        cout << ") " + ops [ i + 1 ];
    }
    return 0;
}
