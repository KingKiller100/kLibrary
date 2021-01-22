#pragma once
#include "../../Localization/kLocale.hpp"

#include "../kStringTypes.hpp"
#include "../../Enum/kEnum.hpp"
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString
{
	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_Char_V<CharT>
		>>
		USE_RESULT constexpr bool IsUpper(CharT c) noexcept
	{
		return kLocale::Query(c, std::ctype_base::upper);
	}

	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_Char_V<CharT>
		>>
		USE_RESULT constexpr bool IsLower(CharT c) noexcept
	{
		return kLocale::Query(c, std::ctype_base::lower);
	}

	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_Char_V<CharT>
		>>
		USE_RESULT constexpr CharT ToUpper(CharT c)
	{
		if (IsLower(c))
		{
			const auto& facet = kLocale::GetFacet<std::ctype<CharT>>();
			c = facet.toupper(c);
		}
		return c;
	}

	template<class Stringish
		, class = std::enable_if_t<
		type_trait::Is_String_V<Stringish>
		|| (type_trait::Is_Char_V<type_trait::Simplify_t<Stringish>>
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
		type_trait::Is_Char_V<CharT>
		&& std::is_array_v<CharT[Size]>
		>>
		USE_RESULT constexpr auto ToUpper(const CharT(&input)[Size])
	{
		return ToUpper(StringReader<std::remove_all_extents_t<CharT>>(input));
	}

	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_Char_V<CharT>
		>>
		USE_RESULT constexpr CharT ToLower(CharT c)
	{
		if (IsUpper(c))
		{
			const auto& facet = kLocale::GetFacet<std::ctype<CharT>>();
			c = facet.tolower(c);
		}
		return c;
	}

	template<class Stringish
		, class = std::enable_if_t<
		type_trait::Is_String_V<Stringish>
		|| (type_trait::Is_Char_V<type_trait::Simplify_t<Stringish>>
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
		type_trait::Is_Char_V<CharT>
		&& std::is_array_v<CharT[Size]>
		>>
		USE_RESULT constexpr auto ToLower(const CharT(&input)[Size])
	{
		return ToLower(StringReader<std::remove_all_extents_t<CharT>>(input));
	}
	

	ENUM_CLASS(CaseSensitive, std::uint8_t, YES, NO);
}