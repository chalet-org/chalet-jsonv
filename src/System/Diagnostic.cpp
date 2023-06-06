#include "System/Diagnostic.hpp"

#include "Libraries/Format.hpp"
#include "System/TerminalColor.hpp"

#include <csignal>
#include <exception>

namespace cjv
{
namespace
{
/*****************************************************************************/
static struct : std::exception
{
	const char* what() const throw() final
	{
		return "A critical error occurred. Review output above";
	}
} kCriticalError;

/*****************************************************************************/
static struct
{
	bool assertionFailure = false;
} state;
}

/*****************************************************************************/
void Diagnostic::warn(const std::string& inMessage)
{
	std::cout << inMessage << std::endl;
}

/*****************************************************************************/
void Diagnostic::error(const std::string& inMessage)
{
	std::cerr << inMessage << std::endl;
}

/*****************************************************************************/
void Diagnostic::errorAbort(const std::string& inMessage)
{
	std::cerr << inMessage << std::endl;
	priv::Diagnostic::abortProgram();
}

namespace priv
{
/*****************************************************************************/
void Diagnostic::customAssertion(const std::string_view inExpression, const std::string_view inMessage, const std::string_view inFile, const u32 inLineNumber)
{
	const auto boldRed = Output::getAnsiStyle(TerminalColor::RedBold);
	const auto colorAt = Output::getAnsiStyle(TerminalColor::BrightMagentaInverted);
	const auto gray = Output::getAnsiStyle(TerminalColor::BrightBlack);
	const auto blue = Output::getAnsiStyle(TerminalColor::BrightBlue);
	const auto reset = Output::getAnsiStyle(TerminalColor::Reset);

	auto header = fmt::format("{}Assertion Failed:{}", boldRed, reset);
	auto location = fmt::format("{} at {} {} {}{}:{}{}", colorAt, reset, inExpression, blue, inFile, inLineNumber, reset);

	cjv::Diagnostic::error(header);
	cjv::Diagnostic::error(location);

	if (!inMessage.empty())
	{
		auto message = fmt::format("{} ..  {}{}", gray, inMessage, reset);
		cjv::Diagnostic::error(message);
	}

	state.assertionFailure = true;
	std::abort();
}

/*****************************************************************************/
bool Diagnostic::assertionFailure() noexcept
{
	return state.assertionFailure;
}

/*****************************************************************************/
void Diagnostic::fatalErrorFromException(const char* inError)
{
	cjv::Diagnostic::error(std::string(inError));
}

/*****************************************************************************/
void Diagnostic::abortProgram()
{
#if defined(CJV_DEBUG)
	::raise(SIGABRT);
#else
	::exit(CJV_EXIT_FAILURE);
#endif
}
}
}