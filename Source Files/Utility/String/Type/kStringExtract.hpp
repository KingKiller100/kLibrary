#pragma once

#include "../kStringTypes.hpp"

#include "../Stringify/kStringifyFloatingPoint.hpp"
#include "../Stringify/kStringifyInteger.hpp"

#include <any>

namespace klib::kString::stringify
{
	template<typename CharT, typename T>
	void ExtractAndInsertInOutput(const std::any& container, StringWriter<CharT>& specifier
		, StringWriter<CharT>& outFinalString, StringWriter<CharT>& outCurrentSection)
	{
		auto data = std::any_cast<T*>(container);
		const auto hexMode = Remove(specifier, 'h');
		const auto binaryMode = Remove(specifier, 'b');
		const auto padding = StrTo<size_t>(specifier);
		
		StringWriter<CharT> toAdd;
		
		if (hexMode)
			toAdd = stringify::StringIntegralHex<CharT>(*data, padding, CharT('0'));
		else if (binaryMode)
			toAdd = stringify::StringIntegralBinary<CharT, ONLY_TYPE(T)>(*data, padding, CharT('0'));
		else
			toAdd = stringify::StringIntegral<CharT, ONLY_TYPE(T)>(*data, padding, CharT('0'));

		
		outCurrentSection.append(toAdd);
		outFinalString.append(outCurrentSection);
	}

	template<typename CharT>
	void ExtractCharAndInsertInOutput(const std::any& container, [[maybe_unused]] const StringWriter<CharT>& specifier
		, StringWriter<CharT>& outFinalString, StringWriter<CharT>& outCurrentSection)
	{
		auto data = std::any_cast<CharT*>(container);
		outCurrentSection.push_back(*data);
		outFinalString.append(outCurrentSection);
	}
	
	template<typename CharT, typename T>
	void ExtractFloatAndInsertInOutput(const std::any& container, StringWriter<CharT>& specifier
		, StringWriter<CharT>& outFinalString, StringWriter<CharT>& outCurrentSection)
	{
		auto data = std::any_cast<T*>(container);
		bool specified = false;

		std::chars_format fmt = std::chars_format::fixed;
		if (Contains(specifier, CharT('e')))
		{
			specified = true;
			fmt = std::chars_format::scientific;
		}
		else if (Contains(specifier, CharT('h')))
		{
			specified = true;
			fmt = std::chars_format::hex;
		}
		else if (Contains(specifier, CharT('g')))
		{
			specified = true;
			fmt = std::chars_format::general;
		}
		else if (specifier, CharT('f'))
			specified = true;

		Remove(specifier, 'g');
		Remove(specifier, 'h');
		Remove(specifier, 'f');
		Remove(specifier, 'e');
		
		const auto padding = StrTo<std::int64_t>(specifier);

		outCurrentSection.append(stringify::StringFloatingPoint<CharT>(*data, padding, fmt));
		outFinalString.append(outCurrentSection);
	}
}
	