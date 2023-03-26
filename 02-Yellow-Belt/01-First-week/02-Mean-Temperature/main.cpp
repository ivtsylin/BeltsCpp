#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;


void task_1_3_1 (const vector<int>& src, vector<int>& dst) {
    dst.resize(0);
    int64_t mean = 0;
    for (const auto& item : src) {
        mean += item;
    }
    mean /= static_cast<int>(src.size());
    for (uint32_t i = 0; i < src.size(); ++i) {
        if (src[i] > mean) {
            dst.push_back(i);
        }
    }
}

int main() {
    int size;
    vector<int> tempr;
    vector<int> res;
    
    cin >> size;
    for (int i = 0, in_var; i < size; ++i) {
        cin >> in_var;
        tempr.push_back(in_var);
    }
    task_1_3_1 (tempr, res);
    
    cout << res.size() << endl;
    for (const auto& item : res) {
        cout << item << " ";
    }
}

