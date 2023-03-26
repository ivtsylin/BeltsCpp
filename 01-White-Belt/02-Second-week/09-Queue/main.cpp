#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int q;
    cin >> q;
    vector<int> queue;
    string command;
    int param;
    for (int i = 0; i < q; ++i) {
        cin >> command;
        if (command == "WORRY_COUNT") {
            int ans_cnt = 0;
            for (auto item : queue) {
                ans_cnt += item;
            }
            cout << ans_cnt << endl;
        } else {
            cin >> param;
            if (command == "WORRY") {
                queue [ param ] = 1;
            } else if (command == "QUIET") {
                queue [ param ] = 0;
            } else if (command == "COME") {
                queue.resize(queue.size() + param, 0);
            }
        }
        
    }
    return 0;
}
