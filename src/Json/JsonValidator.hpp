#pragma once

#include "Libraries/Json.hpp"

#include "Json/JsonValidationError.hpp"

namespace cjv
{
struct JsonValidator
{
	using ValidationErrors = std::vector<JsonValidationError>;

	JsonValidator();
	CJV_DISALLOW_COPY_MOVE(JsonValidator);
	~JsonValidator();

	bool setSchema(Json&& inSchema);

	bool validate(const Json& inJsonContent, const std::string& inFile);
	const ValidationErrors& errors() const noexcept;

	bool printErrors();

private:
	struct Impl;
	Unique<Impl> m_impl;

	ValidationErrors m_errors;
};
}
