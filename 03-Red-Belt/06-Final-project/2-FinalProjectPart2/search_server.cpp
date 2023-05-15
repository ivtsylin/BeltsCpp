#include "search_server.h"
#include "iterator_range.h"
#include "test_runner.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <future>
#include <numeric>

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

InvertedIndex::InvertedIndex(istream& stream) {
    for (string document; getline(stream, document); ) {
        // Дальше метод Add
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
}

vector<pair<int32_t, int32_t>> EMPTY_LOOKUP;

const vector<pair<int32_t, int32_t>>& InvertedIndex::Lookup(const string& word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    }
    return EMPTY_LOOKUP;
}


void UpdateSingleThread(istream& document_input, Synchronized<InvertedIndex>& old_index)
{
    InvertedIndex new_index(document_input);
    swap(old_index.GetAccess().ref_to_value, new_index);
}

void SearchServer::UpdateDocumentBase(istream& document_input)
{
    futures.push_back(async(UpdateSingleThread, ref(document_input), ref(index)));
}

void AddSingleThread(istream &query_input, ostream &search_results_output, Synchronized<InvertedIndex>& index)
{
    vector<int32_t> docs;
    vector<int32_t> found;
    
    for (string current_query; getline(query_input, current_query); ) {
        const vector<string> words = SplitIntoWords(current_query);
        {
            const auto& access = index.GetAccess();
            docs.assign(access.ref_to_value.get_doc_size(), 0);
            found.resize(access.ref_to_value.get_doc_size());
            iota(found.begin(), found.end(), 0);
            
            
            auto& current_index = access.ref_to_value;
            for (const auto& word : words) {
                for (const auto& [docid, count] : current_index.Lookup(word)) {
                    docs[docid] -= count;
                }
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
    }
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output)
{
    futures.push_back(async(AddSingleThread, ref(query_input), ref(search_results_output), ref(index)));
}

