#pragma once

// Exceptions will throw during schema construction otherwise
#if !defined(CJV_EXCEPTIONS)
	#ifndef JSON_NOEXCEPTION
		#define JSON_NOEXCEPTION
	#endif
	#ifndef JSONSV_NOEXCEPTION
		#define JSONSV_NOEXCEPTION
	#endif
#endif

#if defined(CJV_MSVC)
	// #pragma warning(push)
	// #pragma warning(disable : 4100)
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <nlohmann/json.hpp>

#include <nlohmann/json-schema.hpp>

#if defined(CJV_MSVC)
	// #pragma warning(pop)
#else
	#pragma GCC diagnostic pop
#endif

inline nlohmann::ordered_json operator"" _ojson(const char* s, std::size_t n)
{
	return nlohmann::ordered_json::parse(s, s + n);
}

namespace cjv
{
using Json = nlohmann::ordered_json;
using JsonDataType = nlohmann::detail::value_t;
namespace JsonSchema = nlohmann::json_schema;
using JsonSchemaError = JsonSchema::error_descriptor;
}
