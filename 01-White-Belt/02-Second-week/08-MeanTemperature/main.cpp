#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> vec(n);
    for (auto& item : vec) {
        cin >> item;
    }
    int mean_temp = 0;
    for (auto item : vec) {
        mean_temp += item;
    }
    mean_temp /= vec.size();
    vector<int> ans;
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] > mean_temp) {
            ans.push_back(i);
        }
    }
    cout << ans.size() << endl;
    for (auto item : ans) {
        cout << item << " ";
    }
    return 0;
}
