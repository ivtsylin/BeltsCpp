#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    vector<int> ans;
    cin >> n;
    if (n == 0) {
        cout << 0;
    } else {
        int i = 0;
        while (n) {
            ans.push_back(n & 1);
            n >>= 1;
            ++i;
        }
        n = i;
        for (i = n - 1; i >= 0; --i) {
            cout << ans [i];
        }
    }
}

