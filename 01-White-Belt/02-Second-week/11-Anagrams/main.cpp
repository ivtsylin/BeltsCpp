#include <iostream>
#include <map>
#include <string>

using namespace std;

map<char, int> BuildCharCounters (const string& word){
    map<char, int> result;
    for (char c : word) {
        ++result[c];
    }
    return result;
}

int main() {
    int q;
    cin >> q;
    string word1, word2;
    for (int i = 0; i < q; ++i) {
        cin >> word1 >> word2;
        if (BuildCharCounters(word1) == BuildCharCounters(word2)) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
    return 0;
}
