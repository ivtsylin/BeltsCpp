#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool IsPalindrom (const string &str) {
    string rev_str = "";
    for (int i = str.length() - 1; i >= 0; --i) {
        rev_str += str [ i ];
    }
    if (str == rev_str){
        return true;
    }
    return false;
}

vector<string> PalindromFilter (const vector<string> &words, int minLength){
    vector<string> ans;
    for (auto word : words) {
        if (word.length() >= minLength) {
            if (IsPalindrom(word)) {
                ans.push_back(word);
            }
        }
    }
    return ans;
}
