#include <iostream>
#include <string>
#include <vector>

using namespace std;

int Factorial (int n) {
    if (n <= 1) {
        return 1;
    }
    int ans = 1;
    for (int i = 2; i <= n; ++i) {
        ans *= i;
    }
    return ans;
};

int main() {
    int n;
    cin >> n;
    cout << Factorial(n);
    return 0;
}
