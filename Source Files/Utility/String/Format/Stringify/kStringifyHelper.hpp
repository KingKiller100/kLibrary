#pragma once

#include "../../../../TypeTraits/StringTraits.hpp"
#include "../../kStringTricks.hpp"

namespace klib::kString::stringify
{
	constexpr auto s_NoSpecifier = static_cast<size_t>(-1);

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr auto s_DefaultPlaceHolder = CharType('0');

	template<class CharType>
	constexpr std::array<CharType, 16> s_GeneralHexMap = {
		static_cast<CharType>('0'), static_cast<CharType>('1'), static_cast<CharType>('2'), static_cast<CharType>('3'),
		static_cast<CharType>('4'), static_cast<CharType>('5'), static_cast<CharType>('6'), static_cast<CharType>('7'),
		static_cast<CharType>('8'), static_cast<CharType>('9'), static_cast<CharType>('a'), static_cast<CharType>('b'),
		static_cast<CharType>('c'), static_cast<CharType>('d'), static_cast<CharType>('e'), static_cast<CharType>('f'),
	};
	
	template<typename CharType>
	void PrependPadding(std::basic_string<CharType>& outStr, const size_t minDigits, CharType padding)
	{
		if (outStr.size() >= minDigits)
			return;

		const auto count = minDigits - outStr.size();
		outStr.insert(outStr.begin(), count, padding);
	}

	template<typename CharType>
	void PrependPadding(CharType*& outHeadOfPtr, const size_t minDigits, CharType padding)
	{
		const auto size = GetSize(outHeadOfPtr);

		if (size >= minDigits)
			return;

		auto count = minDigits - size;
		while (count > 0)
		{
			*(--outHeadOfPtr) = padding;
			--count;
		}
	}

	template<typename CharType>
	void AppendPadding(std::basic_string<CharType>& outStr, const size_t minDigits, CharType padding)
	{
		if (outStr.size() >= minDigits)
			return;

		const auto count = minDigits - outStr.size();
		outStr.insert(outStr.end(), count, padding);
	}
}
