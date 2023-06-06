#include "Application.hpp"

#if defined(CJV_MSVC)
	#pragma warning(push)
	#pragma warning(disable : 4996)
#endif

#include "Libraries/Format.hpp"
#include "System/TerminalBud.hpp"
#include "Utility/String.hpp"
#include "Json/JsonComments.hpp"
#include "Json/JsonValidator.hpp"

namespace cjv
{
/*****************************************************************************/
i32 Application::run(const i32 argc, const char* argv[])
{
	termbud::initialize();

	if (argc == 1)
	{
		std::cout << "JSON Validator CLI\n"
					 "Usage:\n"
					 "   jsonv -s schema.json path/file1.json path/file2.json\n"
					 "   jsonv -s schema.json path/*.json.json\n";
		return CJV_EXIT_SUCCESS;
	}

	CJV_TRY
	{
		for (i32 i = 1; i < argc; ++i)
		{
			std::string arg(argv[i]);
			if (arg == "-s")
			{
				if (i + 1 == argc)
					CJV_THROW(std::runtime_error("-c passed without file argument"));

				i++;
				std::string nextArg(argv[i]);
				if (String::contains('*', arg))
					CJV_THROW(std::runtime_error("glob found in schema filename"));

				m_schemaFile = std::move(nextArg);
			}
			else
			{
				if (String::contains('*', arg))
				{
					auto it = arg.find('*');
					auto searchPath = arg.substr(0, it);
					if (searchPath.back() == '/' || searchPath.back() == '\\')
						searchPath.pop_back();

					if (!fs::exists(searchPath))
						CJV_THROW(std::runtime_error(fmt::format("Path not found: {}", searchPath)));

					if (!fs::is_directory(searchPath))
						CJV_THROW(std::runtime_error(fmt::format("Path not a directory: {}", searchPath)));

					auto endit = arg.find_last_of('*');
					auto searchSuffix = arg.substr(endit + 1);

					std::error_code ec;
					for (auto& it : fs::directory_iterator(searchPath, ec))
					{
						if (it.is_regular_file())
						{
							const auto& path = it.path();
							const auto extension = path.extension().string();
							if (!String::equals(searchSuffix, extension))
								continue;

							const auto stem = path.stem().string();
							m_files.emplace_back(path.string());
						}
					}
				}
				else
				{
					if (arg[0] == '-')
					{
					}
					if (!fs::exists(arg))
						CJV_THROW(std::runtime_error(fmt::format("Schema path not found: {}", arg)));

					m_files.emplace_back(std::move(arg));
				}
			}
		}

		if (m_schemaFile.empty())
			CJV_THROW(std::runtime_error("No schema file given! Nothing to validate against."));

		if (m_files.empty())
			CJV_THROW(std::runtime_error("No file(s) given! Nothing to validate."));

#if defined(CJV_DEBUG)
		std::cout << "Schema:\n   " << m_schemaFile << '\n';

		std::cout << "Files:\n";
		for (auto& file : m_files)
		{
			std::cout << "   " << file << '\n';
		}
#endif

		if (!validate())
			CJV_THROW(std::runtime_error("Error validating the given file(s)."));

		std::cout << "Success! File(s) passed validation.\n";

		termbud::dispose();
		return CJV_EXIT_SUCCESS;
	}
	CJV_CATCH(const std::exception& e)
	{
		Diagnostic::error(e.what());
		// Clean up what state we can and bail
		termbud::dispose();
		return CJV_EXIT_FAILURE;
	}
}

/*****************************************************************************/
bool Application::validate() const
{
	bool result = true;
	CJV_TRY
	{
		Json schema;
		if (!JsonComments::parse(schema, m_schemaFile))
			return false;

		JsonValidator validator;
		validator.setSchema(std::move(schema));

		for (auto& file : m_files)
		{
			Json jsonFile;
			if (!JsonComments::parse(jsonFile, file))
			{
				result = false;
				continue;
			}

			JsonValidationErrors errors;
			if (!validator.validate(jsonFile, file, errors))
			{
				Diagnostic::error(fmt::format("File: {}", file));
				if (!validator.printErrors(errors))
					result = false;
			}
		}
		return result;
	}
	CJV_CATCH(const std::exception& e)
	{
		CJV_THROW(std::runtime_error(e.what()));
		result = false;
	}

	return result;
}
}

#if defined(CJV_MSVC)
	#pragma warning(pop)
#endif