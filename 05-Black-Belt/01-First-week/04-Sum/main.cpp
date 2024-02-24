#include <iostream>
#include <limits>

using namespace std;

int main() {
    int64_t a, b;
    uint64_t imax = std::numeric_limits<int64_t>::max();
    uint64_t imin = imax + 1;
    
    cin >> a >> b;
    uint64_t abs_a = abs(a), abs_b = abs(b);
    
    if (a >= 0 && b >= 0) {
        if (abs_a + abs_b > imax) {
            cout << "Overflow!";
            return 0;
        }
    } else if (a < 0 && b < 0) {
        if (abs_a + abs_b > imin || abs_a + abs_b == 0) {
            cout << "Overflow!";
            return 0;
        }
    }
    
    cout << a + b;

    return 0;
}
