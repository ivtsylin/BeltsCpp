#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

int main() {
    int q;
    map<string, set<string>> dict_syn;
    cin >> q;
    string cmd;
    string word1, word2;
    for (int i = 0; i < q; ++i) {
        cin >> cmd;
        if (cmd == "ADD") {
            cin >> word1 >> word2;
            dict_syn[word1].insert(word2);
            dict_syn[word2].insert(word1);
        } else if (cmd == "COUNT") {
            cin >> word1;
            if (dict_syn.count(word1)) {
                cout << dict_syn[word1].size() << endl;
            } else {
                cout << 0 << endl;
            }
        } else {
            cin >> word1 >> word2;
            if (dict_syn.count(word1)) {
                if (dict_syn[word1].count(word2)) {
                    cout << "YES" << endl;
                } else {
                    cout << "NO" << endl;
                }
            } else {
                cout << "NO" << endl;
            }
        }
    }
    return 0;
}
