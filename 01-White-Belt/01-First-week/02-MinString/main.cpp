#include <iostream>
#include <string>

using namespace std;

int main() {
    string a, b, c, ans;
    cin >> a >> b >> c;
    if (a <= b && a <= c) {
        ans = a;
    } else if (b <= a && b <= c) {
        ans = b;
    } else {
        ans = c;
    }
    cout << ans;
    return 0;
}
