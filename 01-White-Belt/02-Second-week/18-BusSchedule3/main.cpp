#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

using namespace std;

int main() {
    int q, n;
    cin >> q;
    map<set<string>, int> buses;
    set<string> rout;
    string stop;
    for (int i = 0; i < q; ++i) {
        cin >> n;
        rout.clear();
        for (int j = 0; j < n; ++j) {
            cin >> stop;
            rout.insert(stop);
        }
        if (buses.count(rout)) {
            cout << "Already exists for " << buses[rout] << endl;
        } else {
            n = buses.size() + 1;
            buses[rout] = n;
            cout <<  "New bus " << n << endl;
        }
    }
    return 0;
}
