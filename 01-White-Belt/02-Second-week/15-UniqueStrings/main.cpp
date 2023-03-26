#include <iostream>
#include <string>
#include <set>

using namespace std;

int main() {
    int q;
    set<string> ans;
    string tmp;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        cin >> tmp;
        ans.insert(tmp);
    }
    cout << ans.size();
    return 0;
}
