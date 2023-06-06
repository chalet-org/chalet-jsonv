#pragma once

#include "Libraries/Json.hpp"

namespace cjv
{
namespace JsonComments
{
bool parse(Json& outJson, const std::string& inFilename, const bool inError = true);
Json parseLiteral(const std::string& inJsonContent);
}
}
