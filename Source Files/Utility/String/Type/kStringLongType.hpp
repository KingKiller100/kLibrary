#pragma once

#include "../kStringTypes.hpp"

#include "kStringExtract.hpp"

#include <any>

namespace klib::kString::stringify
{
	template<typename CharT>
	void HandleLongType(StringWriter<CharT>& outFinalString, StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		if (Contains(type, "int"))
		{
			ExtractAndInsertInOutput<CharT, const long int>(container, specifier, outFinalString, outCurrentSection);
		}
		else if (Contains(type, "long long"))
		{
			ExtractAndInsertInOutput<CharT, const long long int>(container, specifier, outFinalString, outCurrentSection);
		}
		else if (Contains(type, "double"))
		{
			ExtractFloatAndInsertInOutput<CharT, const long double>(container, specifier, outFinalString, outCurrentSection);
		}
		else
		{
			ExtractAndInsertInOutput<CharT, const long int>(container, specifier, outFinalString, outCurrentSection);
		}
	}
	
}