#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class SortedStrings {
public:
    SortedStrings () {}
    void AddString(const string& s) {
        Strings.push_back(s);
        sort(Strings.begin(), Strings.end());
    }
    vector<string> GetSortedStrings() {
        // получить набор из всех добавленных строк в отсортированном порядке
        return Strings;
    }
private:
    // приватные поля
    vector<string> Strings;
};

int main() {
    SortedStrings strings;
    vector<string> res;
    strings.AddString("first");
    strings.AddString("third");
    strings.AddString("second");
    res = strings.GetSortedStrings();
    for (const auto &v : res) {
        cout << v << " ";
    }
    cout << endl;
    strings.AddString("second");
    res = strings.GetSortedStrings();
    for (const auto &v : res) {
        cout << v << " ";
    }
    cout << endl;
    return 0;
}
