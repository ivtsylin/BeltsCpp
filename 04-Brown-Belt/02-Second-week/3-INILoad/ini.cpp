#include "ini.h"
#include <iostream>

Ini::Section& Ini::Document::AddSection(string name) {
    return sections.insert({name, {}}).first->second;
}

const Ini::Section& Ini::Document::GetSection(const string& name) const {
    return sections.at(name);
}

size_t Ini::Document::SectionCount() const {
    return sections.size();
}

Ini::Document Ini::Load(istream& input) {
    string line;
    Ini::Document result;
    Ini::Section *cur = nullptr;
    while(getline(input, line)) {
        if (line == "") continue;
        if (line[0] == '[') {
            cur = &result.AddSection(line.substr(1, line.length() - 2));
        } else {
            auto sep = line.find("=");
            (*cur)[line.substr(0, sep)] = line.substr(sep + 1, line.length());
        }
    }
    return result;
}
