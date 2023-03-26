#include <iostream>

using namespace std;

int main() {
    double n, a, b, x, y;
    cin >> n >> a >> b >> x >> y;
    if (n > b) {
        cout << ((1 - y / 100) * n);
    } else if (n > a) {
        cout << ((1 - x / 100) * n);
    } else {
        cout << n;
    }
    return 0;
}

