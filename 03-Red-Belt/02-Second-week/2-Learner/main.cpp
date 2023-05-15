#include "test_runner.h"
#include "profile.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Learner {
public:
    int Learn(const vector<string>& words) {
        int newWords = 0;
        for (const auto& word : words) {
            if (_words.insert(word).second)
                ++newWords;
        }
        return newWords;
    }
    vector<string> KnownWords() {
        return {_words.begin(), _words.end()};
    }
private:
    set<string> _words;
};

/*
int main() {
    LOG_DURATION("Total");
    {
        LOG_DURATION("Empty");
        int a = 0;
    }
    return 0;
}
*/
