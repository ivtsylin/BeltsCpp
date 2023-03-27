#include <string>
#include <fstream>

using namespace std;

int main() {
    string inpath = "input.txt";
    string outpath = "output.txt";
    ifstream input(inpath);
    ofstream output(outpath);
    if (!input.is_open()) {
        cout << "Bad luck" << endl;
    } else {
        string line;
        while (getline(input, line)) {
            output << line << endl;
        }
    }
    return 0;
}

