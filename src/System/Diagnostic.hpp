
#include "System/Types.hpp"

#include "Libraries/Format.hpp"

namespace cjv
{
namespace Diagnostic
{
void warn(const std::string& inMessage);
void error(const std::string& inMessage);
void errorAbort(const std::string& inMessage);
}

namespace priv::Diagnostic
{
void customAssertion(const std::string_view inExpression, const std::string_view inMessage, const std::string_view inFile, const u32 inLineNumber);
void fatalErrorFromException(const char* inError);

bool assertionFailure() noexcept;
void abortProgram();
}
}

#if defined(CJV_DEBUG)
	#define cjv_assert(expr, message) static_cast<void>((expr) || (cjv::priv::Diagnostic::customAssertion(#expr, message, __FILE__, __LINE__), 0))
#else
	#define cjv_assert(expr, message)
#endif
