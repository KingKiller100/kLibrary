#pragma once

#include "kStringExtract.hpp"

#include "../kStringTypes.hpp"
#include "../Stringify/kStringifyInteger.hpp"

#include <any>

namespace klib::kString::impl
{
	constexpr bool IsSimpleInteger(const StringReader<char>& type)
	{
		return Contains(type, "int")
			|| Contains(type, "char")
			|| Contains(type, "short")
			|| Contains(type, "long")
			|| Contains(type, "__int64");
	}

	template<typename CharT>
	void HandleSimpleIntegerType(StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		if (Contains(type, "char"))
		{
			ExtractCharAndInsertInOutput<const CharT>(container, specifier, outCurrentSection);
		}
		else if (Contains(type, "short"))
		{
			ExtractIntegerAndInsertInOutput<CharT, const short>(container, specifier, outCurrentSection);
		}
		else if (Contains(type, "long"))
		{
			if (Contains(type, "long long"))
				ExtractIntegerAndInsertInOutput<CharT, const long long int>(container, specifier, outCurrentSection);
			else
				ExtractIntegerAndInsertInOutput<CharT, const long int>(container, specifier, outCurrentSection);
		}
		else if (Contains(type, "int"))
		{
			if (Contains(type, "__int64"))
				ExtractIntegerAndInsertInOutput<CharT, const __int64>(container, specifier, outCurrentSection);
			else
				ExtractIntegerAndInsertInOutput<CharT, const int>(container, specifier, outCurrentSection);
		}
	}
}

