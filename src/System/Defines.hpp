#pragma once

#if defined(_MSC_VER)
	#define CJV_MSVC
#elif defined(__clang__)
	#define CJV_CLANG
#endif

#if defined(_DEBUG) || defined(DEBUG)
	#ifndef CJV_DEBUG
		#define CJV_DEBUG
	#endif
#endif

#if defined(__APPLE__)
	#ifndef CJV_MACOS
		#define CJV_MACOS
	#endif
#elif defined(__linux__)
	#ifndef CJV_LINUX
		#define CJV_LINUX
	#endif
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#ifndef CJV_WIN32
		#define CJV_WIN32
	#endif
#else
	#error "Unknown platform";
#endif

#if defined(CJV_WIN32)
	#define CJV_EXIT_SUCCESS EXIT_SUCCESS
	#define CJV_EXIT_FAILURE 1067
#else
	#define CJV_EXIT_SUCCESS EXIT_SUCCESS
	#define CJV_EXIT_FAILURE EXIT_FAILURE
#endif

#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND))
	#ifndef CJV_EXCEPTIONS
		#define CJV_EXCEPTIONS
	#endif

	#define CJV_THROW(exception) throw exception
	#define CJV_TRY try
	#define CJV_CATCH(exception) catch (exception)
	#define CJV_EXCEPT_ERROR(what) priv::Diagnostic::fatalErrorFromException(what);
#else
	#include <cstdlib>
	#define CJV_THROW(exception) std::abort()
	#define CJV_TRY if (true)
	#define CJV_CATCH(exception) if (false)
	#define CJV_EXCEPT_ERROR(what)
#endif

#define CJV_DISALLOW_COPY_MOVE(ClassName)            \
	ClassName(const ClassName&) = delete;            \
	ClassName(ClassName&&) noexcept = delete;        \
	ClassName& operator=(const ClassName&) = delete; \
	ClassName& operator=(ClassName&&) noexcept = delete
