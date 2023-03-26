#include <iostream>
#include <vector>

using namespace std;

vector<int> Reversed(const vector<int>& v) {
    vector<int> ans;
    for (int i = v.size() - 1; i >= 0; --i) {
        ans.push_back(v[i]);
    }
    return ans;
}
