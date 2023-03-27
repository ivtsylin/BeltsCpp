#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int q;
    cin >> q;
    vector<int> v;
    int tmp;
    for (int i = 0; i < q; i++) {
        cin >> tmp;
        v.push_back(tmp);
    }
    sort(v.begin(), v.end(), [] (int a, int b) {return abs(a) < abs(b);} );
    for (const auto a : v) {
        cout << a << " ";
    }
    cout << endl;
    return 0;
}
