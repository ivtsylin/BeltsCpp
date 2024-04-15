#pragma once

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <sstream>
#include <iomanip>

namespace Json {

class Node : std::variant<std::vector<Node>,
                          std::map<std::string, Node>,
                          int,
                          std::string,
                          double,
                          bool> {
public:
  using variant::variant;
  // Const type
  const auto& AsArray() const {
      try {
          return std::get<std::vector<Node>>(*this);
      } catch(...) {
          std::stringstream ss;
          ss << ToString() << " try to read AsArray";
          throw std::invalid_argument(ss.str());
      }
  }
  const auto& AsMap() const {
      try {
          return std::get<std::map<std::string, Node>>(*this);
      } catch(...) {
          std::stringstream ss;
          ss << ToString() << " try to read AsMap";
          throw std::invalid_argument(ss.str());
      }
  }
  int AsInt() const {
      try {
          return std::get<int>(*this);
      } catch(...) {
          std::stringstream ss;
          ss << ToString() << " try to read AsInt";
          throw std::invalid_argument(ss.str());
      }
  }
  const auto& AsString() const {
      try {
          return std::get<std::string>(*this);
      } catch(...) {
          std::stringstream ss;
          ss << ToString() << " try to read AsString";
          throw std::invalid_argument(ss.str());
      }
  }
  double AsDouble() const {
      try {
          if (IsInt())
              return std::get<int>(*this);
          return std::get<double>(*this);
      } catch(...) {
          std::stringstream ss;
          ss << ToString() << " try to read AsDouble";
          throw std::invalid_argument(ss.str());
      }
  }
  bool AsBool() const {
      try {
          return std::get<bool>(*this);
      } catch(...) {
          std::stringstream ss;
          ss << ToString() << " try to read AsBool";
          throw std::invalid_argument(ss.str());
      }
  }
  // Getters
  bool IsArray() const {
    return std::holds_alternative<std::vector<Node>>(*this);
  }
  bool IsMap() const {
    return std::holds_alternative<std::map<std::string, Node>>(*this);
  }
  bool IsInt() const {
    return std::holds_alternative<int>(*this);
  }
  bool IsString() const {
    return std::holds_alternative<std::string>(*this);
  }
  bool IsDouble() const {
    return std::holds_alternative<double>(*this);
  }
  bool IsBool() const {
    return std::holds_alternative<bool>(*this);
  }
  // for printing
  std::string ToString() const {
    std::stringstream ss;
    if (IsArray()) {
      ss << '[';
      for (auto& x : AsArray()) ss << x.ToString() << ',';
        auto out = ss.str();
        if (out == "[")
          out.push_back(']');
        else
          out.back() = ']';
        return out;
    } else if (IsMap()) {
      ss << '{';
      for (const auto& [key, value] : AsMap())
        ss << "\"" << key << "\":" << value.ToString() << ',';
        auto out = ss.str();
        if (out == "{")
          out.push_back('}');
        else
          out.back() = '}';
        return out;
    } else if (IsDouble()) {
      ss << std::fixed << std::setprecision(7) << AsDouble();
      return ss.str();
    } else if (IsInt()) {
      ss << AsInt();
      return ss.str();
    } else if (IsString()) {
      ss << '"' << AsString() << '"';
      return ss.str();
    } else if (IsBool())
      return AsBool() ? "true" : "false";
    else
      return "";
  }
};


class Document {
public:
  explicit Document(Node root);

  const Node& GetRoot() const;

private:
  Node root;
};

Document Load(std::istream& input);

std::ostream& operator<<(std::ostream& out, const Document& doc);

}

