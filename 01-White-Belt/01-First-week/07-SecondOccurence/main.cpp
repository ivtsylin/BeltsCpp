#include <iostream>
#include <string>

using namespace std;

int main() {
    string s;
    cin >> s;
    int ans = -2, counter = 0;
    for (auto c : s) {
        if (c == 'f'){
            if (ans == -2){
                ans = -1;
            } else {
                ans = 0;
                cout << counter;
                break;
            }
        }
        ++counter;
    }
    if (ans < 0) {
        cout << ans;
    }
}

