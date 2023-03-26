#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <tuple>


using namespace std;

int FindMaxRepetitionCount (const vector<Region> &src) {
    map<tuple<string, string, map<Lang, string>, int64_t>, int> dst;
    for (const auto& item : src) {
        ++dst[tie(item.std_name, item.parent_std_name, item.names, item.population)];
    }
    int res = 0;
    for (const auto& [key, value] : dst) {
        res = max(res, value);
    }
    return res;
};
