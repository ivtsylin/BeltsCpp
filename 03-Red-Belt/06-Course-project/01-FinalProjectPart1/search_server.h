#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <deque>
using namespace std;

class InvertedIndex {
public:
    
    void Add(const string& document);
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
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    InvertedIndex index;
};
