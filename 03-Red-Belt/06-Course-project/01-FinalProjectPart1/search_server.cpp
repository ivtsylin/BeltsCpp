#include "search_server.h"
#include "iterator_range.h"
#include "test_runner.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <array>
#include <numeric>

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document); ) {
        new_index.Add(move(current_document));
    }

    index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
        
    vector<int32_t> docs(index.get_doc_size());
    vector<int32_t> found(index.get_doc_size());
    iota(found.begin(), found.end(), 0);
    
    for (string current_query; getline(query_input, current_query); ) {
        for (const auto& word : SplitIntoWords(current_query)) {
            for (const auto& [docid, count] : index.Lookup(word)) {
                docs[docid] -= count;
            }
        }
        
        partial_sort(found.begin(), Head(found, 5).end(), found.end(),
                     [&docs](int32_t lhs, int32_t rhs) {
                        return tie(docs[lhs], lhs) < tie(docs[rhs], rhs);
                     }
                     );

        search_results_output << current_query << ':';
        for (auto docid : Head(found,5)) {
            if (docs[docid] == 0) break;
            search_results_output << " {"
                << "docid: " << docid << ", "
                << "hitcount: " << -docs[docid] << '}';
        }
        search_results_output << "\n";
        
        docs.assign(index.get_doc_size(), 0);
        
    }
}

void InvertedIndex::Add(const string& document) {
    for (auto& word : SplitIntoWords(document)) {
        auto &docids = index[move(word)];
        if (docids.empty() || docids.back().first != doc_size) {
            docids.push_back({doc_size, 1});
        } else {
            docids.back().second++;
        }
    }
    doc_size++;
}

vector<pair<int32_t, int32_t>> EMPTY_LOOKUP;

const vector<pair<int32_t, int32_t>>& InvertedIndex::Lookup(const string& word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    }
    return EMPTY_LOOKUP;
}
