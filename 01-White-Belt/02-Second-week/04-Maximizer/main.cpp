#include <iostream>

using namespace std;

void UpdateIfGreater (const int& first, int& second) {
    if (first > second) {
        second = first;
    }
}

int main() {
    int first, second;
    cin >> first >> second;
    UpdateIfGreater (first, second);
    cout << first << " " << second;
    return 0;
}
