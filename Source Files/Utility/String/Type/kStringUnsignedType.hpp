#pragma once

#include "kStringExtract.hpp"
#include "../kStringTypes.hpp"
#include "../Stringify/kStringifyInteger.hpp"

#include <any>


namespace klib::kString::impl
{
	constexpr bool IsUnsigned(const StringReader<char>& type)
	{
		// Both string and string_view will have this name prefix
		return Contains(type, "unsigned");
	}

	template<typename CharT>
	void HandleUnsignedType(StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{		
		if (Contains(type, "char"))
		{
			ExtractIntegerAndInsertInOutput<CharT, const unsigned char>(container, specifier, outCurrentSection);
		}
		else if (Contains(type, "short"))
		{
			ExtractIntegerAndInsertInOutput<CharT, const unsigned short>(container, specifier, outCurrentSection);
		}
		else if (Contains(type, "int"))
		{
			if (Contains(type, "__int64"))
				ExtractIntegerAndInsertInOutput<CharT, const unsigned __int64>(container, specifier, outCurrentSection);
			else
				ExtractIntegerAndInsertInOutput<CharT, const unsigned int>(container, specifier, outCurrentSection);
		}
		else if (Contains(type, "long"))
		{
			if (Contains(type, "long long"))
				ExtractIntegerAndInsertInOutput<CharT, const unsigned long long>(container, specifier, outCurrentSection);
			else
				ExtractIntegerAndInsertInOutput<CharT, const unsigned long>(container, specifier, outCurrentSection);
		}
	}
}
