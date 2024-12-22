#pragma once

#include <utility>
#include <optional>
#include <string>
#include <sstream>
#include <stdexcept>

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view s, std::string_view delimiter = " ");

std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter = " ");

std::string_view ReadToken(std::string_view& s, std::string_view delimiter = " ");

int ConvertToInt(std::string_view str);

double ConvertToDouble(std::string_view str);

