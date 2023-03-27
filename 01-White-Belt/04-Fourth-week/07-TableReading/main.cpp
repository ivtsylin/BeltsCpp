#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main() {
    string path = "input.txt";
    ifstream input(path);
    if (!input.is_open()) {
        cout << "Bad luck" << endl;
    } else {
        int n, m;
        input >> n;
        input.ignore(1);
        input >> m;
        int value;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                input >> value;
                input.ignore(1);
                cout << setw(10);
                cout << value;
                if (j != m - 1) {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }
    return 0;
}
