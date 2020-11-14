#pragma once

#include "kStringExtract.hpp"
#include "../kStringTypes.hpp"
#include "../kStringManipulation.hpp"
#include <any>

namespace klib::kString::type
{
	constexpr bool IsBool(const StringReader<char>& type)
	{
		// Both string and string_view will have this name prefix
		return Contains(type, "bool");
	}

	template<typename CharT>
	void HandleBoolType(StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		ExtractBoolAndInsertInOutput<CharT>(container, specifier, outCurrentSection);
	}
}