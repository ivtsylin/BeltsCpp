#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <sstream>
#include <string_view>
#include <future>
using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (const auto& [key, value] : other.word_frequences) {
            word_frequences[key] += value;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    string_view sv = line, key;
    size_t end_pos = sv.find(' ');
    
    while (end_pos != string::npos) {
        
        key = sv.substr(0, end_pos);
        
        if (key_words.count(string(key))) {
            ++result.word_frequences[string(key)];
        }
        sv.remove_prefix(min(end_pos + 1, sv.length()));
        
        end_pos = sv.find(' ');
    }
    if (key_words.count(string(sv))) {
        ++result.word_frequences[string(sv)];
    }
    return result;
}

Stats ExploreBatch(const set<string>& key_words, const vector<string>& batch) {
    Stats result;
    for (auto& line : batch) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    const size_t MAX_SIZE_BATCH = 5'000;
    vector<future<Stats>> futures;
    vector<string> batch;
    batch.reserve(MAX_SIZE_BATCH);
    Stats result;
    
    int i = 0;
    for (string line; getline(input, line); ++i) {
        batch.push_back(move(line));
        if (batch.size() == MAX_SIZE_BATCH) {
            futures.push_back(async(ExploreBatch, ref(key_words), move(batch)));
            batch.reserve(MAX_SIZE_BATCH);
        }
    }
    if (!batch.empty()) {
        result += ExploreBatch(key_words, move(batch));
    }
    
    for (auto& res : futures) {
        result += res.get();
    }
    // Реализуйте эту функцию
    return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
