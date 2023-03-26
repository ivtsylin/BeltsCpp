#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    int q;
    cin >> q;
    string stop;
    map<vector<string>, int> routs;
    vector<string> rout;
    for (int i = 0; i < q; ++i) {
        int cnt;
        cin >> cnt;
        rout.clear();
        for (int k = 0; k < cnt; ++k) {
            cin >> stop;
            rout.push_back(stop);
        }
        if (routs.count(rout)) {
            cout << "Already exists for " << routs[rout] << endl;
        } else {
            int n = routs.size() + 1;
            routs[rout] = n;
            cout << "New bus " << n << endl;
        }
    }
    return 0;
}
