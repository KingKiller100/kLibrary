#pragma once

#include "kStringExtract.hpp"
#include "../kStringTypes.hpp"

#include <any>

namespace klib::kString::impl
{
	constexpr bool IsFloatingPoint(const StringReader<char>& type)
	{
		return Contains(type, "double") // Includes long double
			|| Contains(type, "float");
	}

	template<typename CharT>
	void HandleFloatingPointType(StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		if (Contains(type, "double"))
		{
			if (Contains(type, "long double"))
			{
				ExtractFloatAndInsertInOutput<CharT, const long double>(container, specifier, outCurrentSection);
			}
			else
			{
				ExtractFloatAndInsertInOutput<CharT, const double>(container, specifier, outCurrentSection);
			}
		}
		else
		{
			ExtractFloatAndInsertInOutput<CharT, const float>(container, specifier, outCurrentSection);
		}
	}

}