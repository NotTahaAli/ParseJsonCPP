#pragma once
#include <string>

std::string TrimString(const std::string& str);

bool isDigit(const char c);

std::string EscapeString(const std::string& str);
std::string* SplitFirst(const std::string& str, const char delim = ':');