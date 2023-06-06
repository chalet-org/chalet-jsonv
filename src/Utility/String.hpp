
#pragma once

namespace cjv
{
namespace String
{
bool equals(const std::string_view inCompare, const std::string& inString);
bool contains(const std::string_view inFind, const std::string& inString);
bool contains(const char inFind, const std::string& inString) noexcept;
void replaceAll(std::string& outString, const char inFrom, const std::string_view inTo);
StringList split(std::string inString, const char inSeparator = ' ', const std::size_t inMinLength = 0);
std::string join(const StringList& inList, const char inSeparator = ' ');
}
}
