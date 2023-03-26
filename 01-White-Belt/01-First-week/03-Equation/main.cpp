#include <iostream>
#include <string>
#include <math.h>

using namespace std;

int main() {
    double a, b, c;
    cin >> a >> b >> c;
    if (a != 0) {
        double dis = b * b - 4 * a * c;
        if (dis > 0) {
            cout << ((-b + sqrt(dis)) / (2 * a)) << " " << ((-b - sqrt(dis)) / (2 * a));
        } else if (dis == 0) {
            cout << (-b / (2 * a));
        }
    } else if (b!=0) {
        cout << (-c / b);
    }
    return 0;
}
