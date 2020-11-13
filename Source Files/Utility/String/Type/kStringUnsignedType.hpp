#pragma once

#include "../kStringTypes.hpp"
#include "../Stringify/kStringifyInteger.hpp"

#include <any>


namespace klib::kString::stringify
{	
	template<typename CharT>
	void HandleUnsignedType(StringWriter<CharT>& outFinalString, StringWriter<CharT>& outCurrentSection
		, const StringReader<char>& type, const std::any& container, StringWriter<CharT>& specifier)
	{
		const auto beforeSection = outCurrentSection;
		
		if (Contains(type, "char"))
		{
			ExtractAndInsertInOutput<CharT, const unsigned char>(container, specifier, outFinalString, outCurrentSection);
		}
		else if (Contains(type, "short"))
		{
			ExtractAndInsertInOutput<CharT, const unsigned short>(container, specifier, outFinalString, outCurrentSection);
		}
		else if (Contains(type, "int"))
		{
			if (Contains(type, "__int64"))
				ExtractAndInsertInOutput<CharT, const unsigned __int64>(container, specifier, outFinalString, outCurrentSection);
			else
				ExtractAndInsertInOutput<CharT, const unsigned int>(container, specifier, outFinalString, outCurrentSection);
		}
		else if (Contains(type, "long"))
		{
			if (Contains(type, "long long"))
				ExtractAndInsertInOutput<CharT, const unsigned long long>(container, specifier, outFinalString, outCurrentSection);
			else
				ExtractAndInsertInOutput<CharT, const unsigned long>(container, specifier, outFinalString, outCurrentSection);
		}
	}
}
