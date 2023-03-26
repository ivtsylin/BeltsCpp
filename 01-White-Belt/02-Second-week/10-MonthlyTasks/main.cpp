#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    const vector<int> num_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int cur_month = 0;
    int next_month = 1;
    vector<vector<string>> HomeTaskList(31);
    int q;
    cin >> q;
    string com;
    string note;
    int param;
    for (int i = 0; i < q; ++i) {
        cin >> com;
        if (com == "ADD") {
            cin >> param >> note;
            HomeTaskList [ param - 1 ].push_back(note);
        } else if (com == "DUMP") {
            cin >> param;
            cout << HomeTaskList [ param - 1 ].size() << " ";
            for (const auto& s : HomeTaskList [ param - 1 ]) {
                cout << s << " ";
            }
            cout << endl;
        } else { // com == NEXT
            next_month = (cur_month + 1) % 12;
            if (cur_month == 0) {
                for (const auto& s : HomeTaskList [ 28 ]) {
                    HomeTaskList [ 27 ].push_back(s);
                }
                for (const auto& s : HomeTaskList [ 29 ]) {
                    HomeTaskList [ 27 ].push_back(s);
                }
                for (const auto& s : HomeTaskList [ 30 ]) {
                    HomeTaskList [ 27 ].push_back(s);
                }
            } else if (num_month[cur_month] > num_month[next_month]) {
                for (const auto& s : HomeTaskList [ 30 ]) {
                    HomeTaskList [ 29 ].push_back(s);
                }
            }
            HomeTaskList.resize(num_month[next_month]);
            cur_month = next_month;
        }
    }
    return 0;
}
