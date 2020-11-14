#pragma once

#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString::stringify
{
	constexpr auto nPrecision = static_cast<size_t>(-1);

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr auto defaultPlaceHolder = CharType('0');

	template<typename CharType>
	void PrependPadding(std::basic_string<CharType>& outStr, const size_t minDigits, CharType padding)
	{
		if (outStr.size() >= minDigits)
			return;

		const auto count = minDigits - outStr.size();
		outStr.insert(outStr.begin(), count, padding);
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
