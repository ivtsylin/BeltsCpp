#include <iostream>
#include <vector>

using namespace std;

void Reverse(vector<int>& v) {
    vector<int> copy_v = v;
    v.clear();
    for (int i = copy_v.size() - 1; i >= 0; --i) {
        v.push_back(copy_v[i]);
    }
}
