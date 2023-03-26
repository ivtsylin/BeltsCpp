#include <iostream>

using namespace std;

int main() {
    int n, m, res;
    cin >> n >> m;
    if (n < m) {
        res = n;
        n = m;
        m = res;
    }
    res = n % m;
    while (res) {
        res = n;
        n = m;
        m = res % m;
        res = n % m;
    }
    cout << m;
}

