#pragma once

#include "json_node.h"

namespace json {

std::istream &operator>>(std::istream &in, Node &node);

namespace json_errros {
static constexpr char ReadStringError[] = "Parsing string is failed";
static constexpr char ReadBoolError[] = "Parsing bool is failed";
static constexpr char ReadNullError01[] = "Parsing the null is failed: too short message";
static constexpr char ReadNullError02[] = "Parsing the null is failed: message is not \"null\"";
static constexpr char ReadNumberError01[] = "Parsing the int is failed";
static constexpr char ReadNumberError02[] = "Parsing the double is failed";
static constexpr char ReadArrayError01[] = "Parsing the array is failed";
static constexpr char ReadArrayError02[] = "Parsing an item in the array is failed: can not read construction Node+Delimiter";
static constexpr char ReadArrayError03[] = "Parsing the array is failed: Delimiter is incorrect";
static constexpr char ReadMapError01[] = "Parsing an item: Can not find the next symbol";
static constexpr char ReadMapError02[] = "Parsing an item: Key-value combination is invalid";
static constexpr char ReadMapError03[] = "Parsing an item: Key is not string";
static constexpr char ReadMapError04[] = "Parsing an item: Key is duplicated";
}

}
