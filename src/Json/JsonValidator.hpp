#pragma once

#include "Libraries/Json.hpp"

#include "Json/JsonValidationError.hpp"

namespace cjv
{
struct JsonValidator
{

	JsonValidator();
	CJV_DISALLOW_COPY_MOVE(JsonValidator);
	~JsonValidator();

	bool setSchema(Json&& inSchema);

	bool validate(const Json& inJsonContent, const std::string& inFile, JsonValidationErrors& errors);

	bool printErrors(JsonValidationErrors& errors);

private:
	struct Impl;
	Unique<Impl> m_impl;
};
}
