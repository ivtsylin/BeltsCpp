//#include "test_runner.h"

#include <map>
#include <list>
#include <iostream>
using namespace std;


class Athlete_List {
public:
    void Add (int a, int b) {
        if (index.count(b)) {
            result.insert(index.at(b), a);
            index[a] = prev(index.at(b));
        } else {
            result.push_back(a);
            index[a] = prev(result.end());
        }
    }
    const auto begin() const {
        return result.begin();
    }
    const auto end() const {
        return result.end();
    }
private:
    map<int, list<int>::iterator> index;
    list<int> result;
};

int main() {
    Athlete_List athlets;
    int num;
    cin >> num;
    int a, b;
    while (num--) {
        cin >> a >> b;
        athlets.Add(a, b);
    }
    for (const auto& athlet : athlets) {
        cout << athlet << endl;
    }

    return 0;
}
