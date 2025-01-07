#include "json_node_reader.h"
#include <sstream>
#include <iomanip>
#include <exception>
#include <cctype>
#include <cmath>

using namespace std;
using namespace json;

std::istream &ReadString(std::istream &input, Node &str) {
    std::string line;
    
    if (!getline(input, line, '"')) {
        input.setstate(std::ios_base::failbit);
        throw runtime_error(json_errros::ReadStringError);
        // "Parsing string is failed"
        return input;
    }
    str = std::move(line);
    return input;
}

std::istream &ReadArray(std::istream &input, Node &arr) {
    Array result;
    char c;
    
    if (!(input >> c)) {
        input.setstate(std::ios_base::failbit);
        throw runtime_error(json_errros::ReadArrayError01);
        return input;
    }
    if (c == ']') {
        arr = result;
        return input;
    }
    input.putback(c);

    do {
        Node node;
        if (!(input >> node >> c)) {
            input.setstate(std::ios_base::failbit);
            throw runtime_error(json_errros::ReadArrayError02);
            return input;
        }
        result.push_back(std::move(node));
        if (c == ']') {
            arr = std::move(result);
            return input;
        }
    } while (c == ',');
    
    input.setstate(std::ios_base::failbit);
    throw runtime_error(json_errros::ReadArrayError03);
    return input;
}

std::istream &ReadBool(std::istream &input, Node &out) {
    bool b;
    input >> std::boolalpha >> b;
    if (!input) {
        input.setstate(std::ios_base::failbit);
        throw runtime_error(json_errros::ReadBoolError);
        return input;
    }
    out = Node{b};
    return input;
}

std::istream &ReadNull(std::istream &input, Node &null) {
    char n, u, l1, l2;
    if (!input.get(n).get(u).get(l1).get(l2)) {
        input.setstate(std::ios_base::failbit);
        throw runtime_error(json_errros::ReadNullError01);
        return input;
    }
    if (n != 'n' || u != 'u' || l1 != 'l' || l2 != 'l') {
        input.setstate(std::ios_base::failbit);
        throw runtime_error(json_errros::ReadNullError02);
        return input;
    }
    null = Node{std::monostate{}};
    return input;
}

std::istream &ReadNumber(std::istream &input, Node &num) {
    int integer = 0;
    double real = 0.;
    int sign = 1;
    if (input.peek() == '-') {
        sign = -1;
    }

    input >> integer;
    if (!input) {
        input.setstate(std::ios_base::failbit);
        throw runtime_error(json_errros::ReadNumberError01);
        // "Parsing the int is failed"
        return input;
    }
    if (input.peek() == '.') {
        input >> real;
        if (!input) {
            input.setstate(std::ios_base::failbit);
            throw runtime_error(json_errros::ReadNumberError02);
            // "Parsing the double is failed"
            return input;
        }
        num = integer + sign * real;
        return input;
    }
    num = integer;
    return input;
}

std::istream &ReadMap(std::istream &input, Node &dict) {
    Map result;
    char comma, c1, c2;
    do {
        if (!(input >> comma)) {
            input.setstate(std::ios_base::failbit);
            throw runtime_error(json_errros::ReadMapError01);
            // "Parsing an item: Can not find the next symbol"
            return input;
        }
        if (comma == '}') {
            dict = result;
            return input;
        } else {
            input.putback(comma);
        }

        Node key, value;
        input >> key >> c1 >> value >> c2;
        if (!input) {
            input.setstate(std::ios_base::failbit);
            throw runtime_error(json_errros::ReadMapError02);
            // "Parsing an item: Key-value combination is invalid"
            return input;
        }
        if (!(key.IsString()) || c1 != ':') {
            input.setstate(std::ios_base::failbit);
            throw runtime_error(json_errros::ReadMapError03);
            // "Parsing an item: Key is not string"
            return input;
        }
        auto [_, inserted] = result.emplace(key.ReleaseString(), value);
        if (!inserted) {
            input.setstate(std::ios_base::failbit);
            throw runtime_error(json_errros::ReadMapError04);
            // "Parsing an item: Key is duplicated"
            return input;
        }
        if (c2 == '}') {
            dict = std::move(result);
            return input;
        }
    } while (c2 == ',');
    return input;
}

std::istream &json::operator>>(std::istream &input, Node &node) {
    char c;
    if (!(input >> c)) return input;

    if (c == '[') {
        ReadArray(input, node);
    } else if (c == '{') {
        ReadMap(input, node);
    } else if (c == '"') {
        ReadString(input, node);
    } else if (c == 't' || c == 'f') {
        input.putback(c);
        ReadBool(input, node);
    } else if (isdigit(c) || c == '-') {
        input.putback(c);
        ReadNumber(input, node);
    } else if (c == 'n') {
        input.putback(c);
        ReadNull(input, node);
    } else {
        input.setstate(std::ios_base::failbit);
    }
    return input;
}
