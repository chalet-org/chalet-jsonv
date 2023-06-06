#include "Json/JsonComments.hpp"

#include "Libraries/FileSystem.hpp"
#include "Libraries/Format.hpp"
#include "System/TerminalColor.hpp"
#include "Utility/String.hpp"

namespace cjv
{
namespace
{
bool printLinesWithError(std::basic_istream<char>& inContents, const char* inError, const std::string& inOutputError)
{
	std::string error(inError);
	auto lastSquareBrace = error.find(']');
	if (lastSquareBrace != std::string::npos)
	{
		error = error.substr(lastSquareBrace + 2);
		error[0] = static_cast<char>(::toupper(static_cast<uchar>(error[0])));
	}

	auto start = error.find("at line ");
	if (start == std::string::npos)
		return false;

	start += 8;
	auto end = error.find(",", start + 1);
	if (end == std::string::npos)
		return false;

	auto lineRaw = error.substr(start, end - start);

	start = error.find("column ", end);
	if (start == std::string::npos)
		return false;

	start += 7;
	end = error.find(":", start + 1);
	if (end == std::string::npos)
		return false;

	auto columnRaw = error.substr(start, end - start);

	if (lineRaw.empty() || columnRaw.empty())
		return false;

	i32 lineNo = 0;
	i32 columnNo = 0;
	CJV_TRY
	{
		lineNo = std::stoi(lineRaw);
		columnNo = std::stoi(columnRaw);
	}
	CJV_CATCH(...)
	{
		return false;
	}

	std::string output = fmt::format("{}\n", error);

	auto colorGray = Output::getAnsiStyle(TerminalColor::Gray);
	auto colorError = Output::getAnsiStyle(TerminalColor::Red);
	auto colorReset = Output::getAnsiStyle(TerminalColor::Reset);

	i32 i = 0;
	for (std::string line; std::getline(inContents, line); ++i)
	{
		if (i >= lineNo - 4 && i <= lineNo + 2)
		{
			if (i > 0)
				output += '\n';

			bool current = i == lineNo - 1;
			auto& color = current ? colorError : colorGray;
			std::string outLine;
			if (current)
			{
				std::size_t columnIndex = static_cast<std::size_t>(columnNo - 1);
				for (std::size_t j = 0; j < line.size(); ++j)
				{
					if (j == columnIndex)
						outLine += fmt::format("{}{}{}", colorError, line[j], colorReset);
					else
						outLine += line[j];
				}
			}
			else
			{
				outLine = std::move(line);
			}
			output += fmt::format("{}{} | {}{}", color, i + 1, colorReset, outLine);
		}
	}

	Diagnostic::error(fmt::format("{}", output));
	Diagnostic::error(fmt::format("{}", inOutputError));
	return true;
}
}

/*****************************************************************************/
bool JsonComments::parse(Json& outJson, const std::string& inFilename, const bool inError)
{
	std::error_code ec;
	if (!fs::exists(inFilename, ec))
	{
		outJson = Json();
		return true;
	}

	std::string lines;
	std::ifstream fileStream(inFilename);

	CJV_TRY
	{
		nlohmann::detail::parser_callback_t<Json> cb = nullptr;
		bool allow_exceptions = true;
		bool ignore_comments = true;

		outJson = Json::parse(fileStream, cb, allow_exceptions, ignore_comments);
		return true;
	}
	CJV_CATCH(const std::exception& err)
	{
		if (inError)
		{
			auto error = err.what();
			{
				auto msg = "There was a problem reading the json file";
				std::ifstream jsonFile(inFilename);
				if (!printLinesWithError(jsonFile, error, fmt::format("{}: {}", msg, inFilename)))
				{
					Diagnostic::error(fmt::format("{}", error));
					Diagnostic::error(fmt::format("{}: {}", msg, inFilename));
				}
			}
			outJson = Json();
		}
		return false;
	}
}

/*****************************************************************************/
Json JsonComments::parseLiteral(const std::string& inJsonContent)
{
	CJV_TRY
	{
		nlohmann::detail::parser_callback_t<Json> cb = nullptr;
		bool allow_exceptions = true;
		bool ignore_comments = true;

		return Json::parse(inJsonContent, cb, allow_exceptions, ignore_comments);
	}
	CJV_CATCH(const std::exception& err)
	{
		auto error = err.what();
		{
			auto msg = "There was a problem reading the json";
			std::stringstream stream;
			stream << inJsonContent;
			if (!printLinesWithError(stream, error, fmt::format("{}", msg)))
			{
				Diagnostic::error(fmt::format("{}", error));
				Diagnostic::error(fmt::format("{}", msg));
			}
		}
		return Json();
	}
}
}