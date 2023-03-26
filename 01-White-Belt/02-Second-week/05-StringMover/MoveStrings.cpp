#include <iostream>
#include <vector>

using namespace std;

void MoveStrings (vector<string>& source, vector<string>& destination) {
    for (auto item : source) {
        destination.push_back(item);
    }
    source.clear();
}
