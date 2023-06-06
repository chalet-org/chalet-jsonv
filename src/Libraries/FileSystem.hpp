#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1920
#elif defined(__cplusplus) && __cplusplus >= 201703L && defined(__has_include)
	#if __has_include(<filesystem>)
		#if __clang_major__ >= 11 || __GNUC__ >= 9
		#else
			#define CJV_NO_FILE_SYSTEM
		#endif
	#else
		#define CJV_NO_FILE_SYSTEM
	#endif
#else
	#define CJV_NO_FILE_SYSTEM
#endif

#if defined(CJV_NO_FILE_SYSTEM)
	#error "A compiler with at least C++17 and std::filesystem is required"
#else
	#include <filesystem>
namespace cjv
{
namespace fs = std::filesystem;
}
#endif
