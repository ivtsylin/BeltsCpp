#include "json_node_writer.h"
#include <sstream>
#include <iomanip>

using namespace std;
using namespace json;

void Write(std::ostream &out, std::monostate) {
    out << "null";
}

void Write(std::ostream &out, bool value) {
    out << std::boolalpha << value;
}

void Write(std::ostream &out, const std::string &value) {
    out << std::quoted(value);
}

template<typename T>
void Write(std::ostream &out, const T &value) {
    out << value;
}

void Write(std::ostream &out, const json::Array &list) {
    out << '[';
    bool first = true;
    for (auto &item : list) {
        if (!first) out << ",";
        first = false;
        Write(out, item);
    }
    out << ']';
}

void Write(std::ostream &out, const json::Map &list) {
    out << '{';
    bool first = true;
    for (auto &[key, value] : list) {
        if (!first) out << ",";
        first = false;
        Write(out, key);
        out << ":" << value;
    }
    out << '}';
}

std::ostream &json::operator<<(std::ostream &out, const Node &node) {
    std::visit([&](auto &&val) { Write(out, val); }, node.GetValue());
    return out;
}
