#include <map>
#include <string>
#include <set>

using namespace std;

set<string> BuildMapValuesSet (const map<int, string>& dict) {
    set<string> ans;
    for (const auto& [key, value] : dict) {
        ans.insert(value);
    }
    return ans;
}


