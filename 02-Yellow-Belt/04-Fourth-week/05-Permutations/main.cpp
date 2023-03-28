#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


void generate_string (vector<string> &result, int N, string a="", int k = 0);

void generate_string (vector<string> &result, int N, string a, int k) {
    if (k >= N) {
        result.push_back(a);
        return;
    }
    
    for (int i = 0; i < (k + 1); ++i) {
        generate_string (result, N, a.substr(0, i) + to_string(k + 1) + a.substr(i, k - i), k + 1);
    }
}

int main() {
    int N;
    cin >> N;
    vector<string> result;
    generate_string (result, N);
    sort(result.rbegin(), result.rend());
    uint64_t last_string = 0;
    int last_symbol = 0;
    for (const auto& item : result) {
        last_string++;
        for (const auto& ch : item) {
            last_symbol++;
            cout << ch;
            if (last_symbol != N) {
                cout << " ";
            }
        }
        last_symbol = 0;
        if (last_string != result.size()) {
            cout << endl;
        }
    }
}

