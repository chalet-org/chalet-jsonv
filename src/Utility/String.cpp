#include "Utility/String.hpp"

namespace cjv
{
/*****************************************************************************/
bool String::equals(const std::string_view inCompare, const std::string& inString)
{
	return inString == inCompare;
}

/*****************************************************************************/
bool String::contains(const std::string_view inFind, const std::string& inString)
{
	return inString.find(inFind) != std::string::npos;
}

/*****************************************************************************/
bool String::contains(const char inFind, const std::string& inString) noexcept
{
	return inString.find(inFind) != std::string::npos;
}

/*****************************************************************************/
void String::replaceAll(std::string& outString, const char inFrom, const std::string_view inTo)
{
	std::size_t pos = 0;
	while ((pos = outString.find(inFrom, pos)) != std::string::npos)
	{
		outString.replace(pos, 1, inTo);
		pos += inTo.length();
	}

	return;
}

/*****************************************************************************/
StringList String::split(std::string inString, const char inSeparator, const std::size_t inMinLength)
{
	StringList ret;

	std::string sub;

	std::size_t itr = 0;
	std::size_t nextNonChar = 0;

	bool nonCharFound = false;
	while (itr != std::string::npos)
	{
		itr = inString.find(inSeparator);

		sub = inString.substr(0, itr);
		nextNonChar = inString.find_first_not_of(inSeparator, itr);

		nonCharFound = nextNonChar != std::string::npos;
		inString = inString.substr(nonCharFound ? nextNonChar : itr + 1);
		if (nonCharFound)
			itr = nextNonChar;

		if (!sub.empty())
		{
			while (sub.back() == inSeparator)
				sub.pop_back();
		}

		if (sub.size() >= inMinLength)
			ret.push_back(sub);
	}

	return ret;
}

/*****************************************************************************/
std::string String::join(const StringList& inList, const char inSeparator)
{
	std::string ret;

	std::ptrdiff_t i = 0;
	for (auto& item : inList)
	{
		if (item.empty())
			continue;

		i = &item - &inList.front();
		if (i > 0)
			ret += inSeparator;

		ret += item;
	}

	return ret;
}
}