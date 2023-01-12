#pragma once
#include <vector>
#include <string>

std::vector<std::string_view> split(std::string_view input, std::string_view delimiter, bool include_delimiter);
std::string to_lower(std::string_view s);