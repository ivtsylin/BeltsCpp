#pragma once

#include "synchronized.h"

#include <istream>
#include <ostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <deque>
#include <string_view>
#include <future>

using namespace std;

class InvertedIndex {
public:
    InvertedIndex() = default;
    explicit InvertedIndex(istream& input);
    const vector<pair<int32_t, int32_t>>& Lookup(const string& word) const;
    int32_t get_doc_size() const {
        return doc_size;
    }
private:
    unordered_map<string, vector<pair<int32_t, int32_t>>> index;
    int32_t doc_size = 0;
};


class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input) : index(InvertedIndex(document_input)) {};
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    Synchronized<InvertedIndex> index;
    vector<future<void>> futures;
};
