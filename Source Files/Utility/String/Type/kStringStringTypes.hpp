#pragma once

#include "kStringExtract.hpp"
#include "../kStringTypes.hpp"
#include "../kStringManipulation.hpp"
#include <any>

namespace klib::kString::type
{
	constexpr bool IsSTLString(const StringReader<char>& type)
	{
		// Both string and string_view will have this name prefix
		return Contains(type, "basic_string");
	}

	template<typename CharT>
	void HandleStringType(StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		if (Contains(type, "basic_string_view"))
		{
			ExtractStringAndInsertInOutput<const std::basic_string_view<CharT>>(container, specifier, outCurrentSection);
		}
		else if (Contains(type, "basic_string"))
		{
			ExtractStringAndInsertInOutput<const std::basic_string<CharT>>(container, specifier, outCurrentSection);
		}
	}
}
