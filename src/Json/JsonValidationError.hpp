#pragma once

#include "Libraries/Json.hpp"

#include "Json/JsonErrorClassification.hpp"

namespace cjv
{
struct JsonValidationError
{
	std::string key;
	std::string value;
	std::string typeName;
	std::string message;
	StringList tree;
	std::any data;
	JsonSchemaError type = JsonSchemaError::none;
	JsonErrorClassification classification = JsonErrorClassification::None;
};

using JsonValidationErrors = std::vector<JsonValidationError>;
}