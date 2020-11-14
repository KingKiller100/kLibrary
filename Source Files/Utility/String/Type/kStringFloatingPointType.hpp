#pragma once

#include "../kStringTypes.hpp"


#include "kStringExtract.hpp"

#include <any>

namespace klib::kString::stringify
{
	inline bool IsFloatingPoint(const StringReader<char>& type)
	{
		return Contains(type, "double")
			|| Contains(type, "float");
	}
	
	template<typename CharT>
	void HandleFloatingPointType(StringWriter<CharT>& outFinalString, StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		if (Contains(type, "double"))
		{
				ExtractFloatAndInsertInOutput<CharT, const double>(container, specifier, outFinalString, outCurrentSection);
		}
		else
		{
			ExtractFloatAndInsertInOutput<CharT, const float>(container, specifier, outFinalString, outCurrentSection);
		}
	}

}