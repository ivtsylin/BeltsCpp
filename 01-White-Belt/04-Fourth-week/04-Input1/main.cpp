#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    string path = "input.txt";
    ifstream input(path);
    if (!input.is_open()) {
        cout << "Bad luck" << endl;
    } else {
        string line;
        while (getline(input, line)) {
            cout << line << endl;
        }
    }
    return 0;
}

