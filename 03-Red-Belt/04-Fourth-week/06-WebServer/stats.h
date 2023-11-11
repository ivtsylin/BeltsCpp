#pragma once

#include "http_request.h"

#include <map>
#include <set>
#include <string_view>
#include <string>

using namespace std;

class Stats {
public:
    Stats();
    void AddMethod(string_view method);
    void AddUri(string_view uri);
    const map<string_view, int>& GetMethodStats() const;
    const map<string_view, int>& GetUriStats() const;
private:
    map<string_view, int> dict_method, dict_uri;
    set<string_view> known_methods, known_uris;
    
};

HttpRequest ParseRequest(string_view line);
