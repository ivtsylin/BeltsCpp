#pragma once

#include "stats.h"

using namespace std;

Stats::Stats() {
    known_methods = {"GET", "PUT", "POST", "DELETE"};
    dict_method = {{"GET", 0}, {"PUT", 0}, {"POST", 0}, {"DELETE", 0}, {"UNKNOWN", 0}};
    known_uris = {"/", "/order", "/product", "/basket", "/help", "unknown"};
    dict_uri = {{"/", 0}, {"/order", 0}, {"/product", 0}, {"/basket", 0}, {"/help", 0}, {"unknown", 0}};
}

void Stats::AddMethod(string_view method) {
    if (known_methods.count(method)) {
        ++dict_method[method];
    } else {
        ++dict_method["UNKNOWN"];
    }
}

void Stats::AddUri(string_view uri) {
    if (known_uris.count(uri)) {
        ++dict_uri[uri];
    } else {
        ++dict_uri["unknown"];
    }
}

const map<string_view, int>& Stats::GetMethodStats() const {
    return dict_method;
}

const map<string_view, int>& Stats::GetUriStats() const {
    return dict_uri;
}

HttpRequest ParseRequest(string_view line) {
    HttpRequest result;
    
    line.remove_prefix(line.find_first_not_of(" "));
    
    size_t str_end = line.find(' ');
    result.method = line.substr(0, str_end);
    line.remove_prefix(min(str_end + 1, line.length()));
    
    str_end = line.find(' ');
    result.uri = line.substr(0, str_end);
    line.remove_prefix(min(str_end + 1, line.length()));
    
    result.protocol = line.substr(0, line.find(' '));
    
    return result;
}
