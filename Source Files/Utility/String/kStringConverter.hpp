#pragma once

#include "../../HelperMacros.hpp"
#include "../../Type Traits/StringTraits.hpp"
#include <string>

namespace klib::kString
{
	template<typename DestChar, typename SourceChar, class = std::enable_if_t<
		type_trait::Is_CharType_V<DestChar>
		&& type_trait::Is_CharType_V<SourceChar>
		>>
		USE_RESULT constexpr DestChar Convert(const SourceChar source) noexcept
	{
		return CAST(DestChar, source);
	}
	
	template<typename DestChar, typename SourceChar, class = std::enable_if_t<
		type_trait::Is_CharType_V<DestChar>
		&& type_trait::Is_CharType_V<SourceChar>
		>>
		USE_RESULT constexpr DestChar* Convert(const SourceChar* source) noexcept
	{
		if _CONSTEXPR_IF(std::is_same_v<SourceChar, DestChar>)
			return (DestChar*)source;
		
		constexpr auto destTerminator = DestChar();
		constexpr auto sourceTerminator = SourceChar();

		auto currentPos = source;
		size_t size = 0;

		while (*(currentPos++) != sourceTerminator)
			size++;

		auto result = new DestChar[size + 1]{ destTerminator };
		do {
			result[size] = CAST(DestChar, source[size]);
		} while (size-- > 0);

		return result;
	}

	template<class DestChar, class SourceChar, class = std::enable_if_t <
		type_trait::Is_CharType_V<DestChar>
		&& type_trait::Is_CharType_V<SourceChar>
		&& type_trait::Is_StringType_V<std::basic_string<DestChar>>
		&& type_trait::Is_StringType_V<std::basic_string<SourceChar>>
		>>
		USE_RESULT std::basic_string<DestChar> Convert(const std::basic_string<SourceChar>& source) noexcept
	{
		return Convert<DestChar>(source.data());
	}

	template<class DestChar, class SourceChar, class = std::enable_if_t <
		type_trait::Is_CharType_V<DestChar>
		&& type_trait::Is_CharType_V<SourceChar>
		&& type_trait::Is_StringType_V<std::basic_string_view<DestChar>>
		&& type_trait::Is_StringType_V<std::basic_string_view<SourceChar>>
		>>
		USE_RESULT std::basic_string<DestChar> Convert(const std::basic_string_view<SourceChar>& source) noexcept
	{
		return Convert<DestChar>(source.data());
	}
}
