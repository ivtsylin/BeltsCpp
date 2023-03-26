#include <iostream>

using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    if ((a + (a & 1)) <= b) {
        cout << (a + (a & 1));
    }
    for (int i = a + (a & 1) + 2; i <= b; i += 2) {
        cout << " " << i;
    }
    return 0;
}

