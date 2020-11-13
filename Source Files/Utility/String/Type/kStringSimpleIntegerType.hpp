#pragma once

#include "../kStringTypes.hpp"

#include "../Stringify/kStringifyInteger.hpp"

#include <any>

namespace klib::kString::stringify
{
	inline bool IsSimpleInteger(const StringReader<char>& type)
	{
		return type == "int"
			|| type == "char"
			|| type == "short"
			|| type == "__int64";
	}

	template<typename CharT>
	void HandleSimpleIntegerType(StringWriter<CharT>& outFinalString, StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		if (Contains(type, "char"))
		{
			ExtractCharAndInsertInOutput<const CharT>(container, specifier, outFinalString, outCurrentSection);
		}
		else if (Contains(type, "short"))
		{
			ExtractAndInsertInOutput<CharT, const short>(container, specifier, outFinalString, outCurrentSection);
		}
		else if (Contains(type, "int"))
		{
			if (Contains(type, "__int64"))
				ExtractAndInsertInOutput<CharT, const __int64>(container, specifier, outFinalString, outCurrentSection);
			else
				ExtractAndInsertInOutput<CharT, const int>(container, specifier, outFinalString, outCurrentSection);
		}
	}
}

