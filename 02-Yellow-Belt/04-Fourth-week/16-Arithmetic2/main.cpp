#include <iostream>
#include <string>
#include <deque>

using namespace std;

int main () {
    deque<string> result;
    string ans;
    cin >> ans;
    result.push_back(ans);
    
    int N;
    cin >> N;
    string op_prev = "*", op_cur, rhs;
    for (int i = 0; i < N; ++i) {
        cin >> op_cur >> rhs;
        if ((op_cur == "*" || op_cur == "/") && (op_prev == "+" || op_prev == "-")) {
            result.push_front("(");
            result.push_back(") " + op_cur + " " + rhs);
        } else {
            result.push_back(" " + op_cur + " " + rhs);
        }
        op_prev = op_cur;
    }
    for (const auto& item : result) {
        cout << item;
    }
    return 0;
}
