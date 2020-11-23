#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

#include "../kStringTypes.hpp"

namespace klib::kString
{
	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr CharT ToUpper(CharT c)
	{
		if (c >= CharT('a') && c <= CharT('z'))
			c -= 32;
		return c;
	}

	template<class Stringish
		, class = std::enable_if_t<
		type_trait::Is_StringType_V<Stringish>
		|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)>
			&& std::is_pointer_v<Stringish>)
		>>
		USE_RESULT constexpr auto ToUpper(const Stringish& input)
	{
		auto output = ToWriter(input);
		for (auto& c : output)
			c = ToUpper(c);
		return output;
	}

	template<class CharT, size_t Size, typename = std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		&& std::is_array_v<CharT[Size]>
		>>
		USE_RESULT constexpr auto ToUpper(const CharT(&input)[Size])
	{
		return ToUpper(StringReader<std::remove_all_extents_t<CharT>>(input));
	}

	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr CharT ToLower(CharT c)
	{
		if (c >= CharT('A') && c <= CharT('Z'))
			c += 32;
		return c;
	}

	template<class Stringish
		, class = std::enable_if_t<
		type_trait::Is_StringType_V<Stringish>
		|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)>
			&& std::is_pointer_v<Stringish>)
		>>
		USE_RESULT constexpr auto ToLower(const Stringish& input)
	{
		auto output = ToWriter(input);
		for (auto& c : output)
			c = ToLower(c);
		return output;
	}

	template<class CharT, size_t Size, typename = std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		&& std::is_array_v<CharT[Size]>
		>>
		USE_RESULT constexpr auto ToLower(const CharT(&input)[Size])
	{
		return ToLower(StringReader<std::remove_all_extents_t<CharT>>(input));
	}
}