#pragma once

#include <istream>
#include <map>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace json {
class Node;
using Map = std::map<std::string, Node>;
using Array = std::vector<Node>;

class Node : std::variant<std::monostate, Array, Map, double, bool, int, std::string> {
public:
    using variant::variant;

    explicit Node(const char *str) {
        *this = std::string(str);
    };
    Node &operator=(const char *str) {
        return *this = std::string(str);
    }
    inline const variant &GetValue() const {
        return *this;
    }

    inline bool IsArray() const {
        return std::holds_alternative<std::vector<Node>>(*this);
    }
    inline bool IsMap() {
        return std::holds_alternative<Map>(*this);
    }
    inline bool IsInt() const {
        return std::holds_alternative<int>(*this);
    }
    inline bool IsDouble() const {
        return std::holds_alternative<double>(*this) || std::holds_alternative<int>(*this);
    }
    inline bool IsBool() const {
        return std::holds_alternative<bool>(*this);
    }
    inline bool IsString() const {
        return std::holds_alternative<std::string>(*this);
    }
    inline bool IsEmpty() const {
        return std::holds_alternative<std::monostate>(*this);
    }

    inline const Array &AsArray() const {
        return std::get<std::vector<Node>>(*this);
    }
    inline const Map &AsMap() const {
        return std::get<Map>(*this);
    }
    inline int AsInt() const {
        return std::get<int>(*this);
    }
    inline double AsDouble() const {
        return std::holds_alternative<double>(*this) ? std::get<double>(*this) : std::get<int>(*this);
    }
    inline bool AsBool() const {
        return std::get<bool>(*this);
    }
    inline const std::string &AsString() const {
        return std::get<std::string>(*this);
    }
    inline const std::monostate &AsMonostate() const {
        return std::get<std::monostate>(*this);
    }
    
    inline Array &AsArray() {
        return std::get<std::vector<Node>>(*this);
    }
    inline Map &AsMap() {
        return std::get<Map>(*this);
    }
    inline int AsInt() {
        return std::get<int>(*this);
    }
    inline double AsDouble() {
        return std::holds_alternative<double>(*this) ? std::get<double>(*this) : std::get<int>(*this);
    }
    inline bool AsBool() {
        return std::get<bool>(*this);
    }
    inline std::string &AsString() {
        return std::get<std::string>(*this);
    }
    
    inline std::string ReleaseString() {
        std::string res = std::move(AsString());
        *this = std::monostate{};
        return res;
    }
    
    inline friend bool operator==(const Node &l, const Node &r) {
        return l.GetValue() == r.GetValue();
    };
    inline friend bool operator!=(const Node &l, const Node &r) {
        return l.GetValue() != r.GetValue();
    };
};
}
