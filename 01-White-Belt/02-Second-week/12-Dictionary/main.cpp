#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    int q;
    cin >> q;
    map<string, string> capital_dictionary;
    string cmd1, cmd2, cmd3;
    for (int i = 0; i < q; ++i) {
        cin >> cmd1;
        if (cmd1 == "CHANGE_CAPITAL") {
            cin >> cmd2 >> cmd3; // Country
            if (!capital_dictionary.count(cmd2)) { // Страны нет еще в словаре
                capital_dictionary[cmd2] = cmd3;
                cout << "Introduce new country " << cmd2 << " with capital " << cmd3 << endl;
            } else { // Страна есть в словаре
                if (capital_dictionary[cmd2] == cmd3) {
                    cout << "Country " << cmd2 << " hasn't changed its capital" << endl;
                } else {
                    cout << "Country " << cmd2 << " has changed its capital from " << capital_dictionary[cmd2] << " to " << cmd3 << endl;
                    capital_dictionary[cmd2] = cmd3;
                }
            }
        } else if (cmd1 == "RENAME") {
            cin >> cmd2 >> cmd3;
            if ((capital_dictionary.count(cmd2) == false) || (capital_dictionary.count(cmd3) == true) || (cmd2 == cmd3)) {
                cout << "Incorrect rename, skip" << endl;
            } else {
                cout << "Country " << cmd2 << " with capital " << capital_dictionary[cmd2] << " has been renamed to " << cmd3 << endl;
                capital_dictionary[cmd3] = capital_dictionary[cmd2];
                capital_dictionary.erase(cmd2);
            }
        } else if (cmd1 == "ABOUT") {
            cin >> cmd2;
            if (!capital_dictionary.count(cmd2)) {
                cout << "Country " << cmd2 << " doesn't exist" << endl;
            } else {
                cout << "Country " << cmd2 << " has capital " << capital_dictionary[cmd2] << endl;
            }
        } else { // cmd1 == "DUMP"
            if (capital_dictionary.size() == 0) {
                cout << "There are no countries in the world" << endl;
            } else {
                for (const auto& [key, value] : capital_dictionary) {
                    cout << key << "/" << value << " ";
                }
                cout << endl;
            }
        }
    }
    return 0;
}
