#pragma once
#include "kStringLocale.hpp"

#include "../kStringTypes.hpp"
#include "../../Enum/kEnum.hpp"
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString
{
	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr bool IsUpper(CharT c, const std::string_view& localeName = "C") noexcept
	{
		return IsIt(c, std::ctype_base::upper, localeName);
	}

	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr bool IsLower(CharT c, const std::string_view& localeName = "C") noexcept
	{
		return IsIt(c, std::ctype_base::lower, localeName);
	}

	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr CharT ToUpper(CharT c, const std::string_view& localeName = "C")
	{
		if (IsLower(c, localeName))
		{
			const auto& facet = impl::GetFacet<std::ctype<CharT>>(localeName);
			c = facet->toupper(c);
		}
		return c;
	}

	template<class Stringish
		, class = std::enable_if_t<
		type_trait::Is_StringType_V<Stringish>
		|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)>
			&& std::is_pointer_v<Stringish>)
		>>
		USE_RESULT constexpr auto ToUpper(const Stringish& input, const std::string_view& localeName = "C")
	{
		auto output = ToWriter(input);
		for (auto& c : output)
			c = ToUpper(c, localeName);
		return output;
	}

	template<class CharT, size_t Size, typename = std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		&& std::is_array_v<CharT[Size]>
		>>
		USE_RESULT constexpr auto ToUpper(const CharT(&input)[Size], const std::string_view& localeName = "C")
	{
		return ToUpper(StringReader<std::remove_all_extents_t<CharT>>(input), localeName);
	}

	template<class CharT, class =
		std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr CharT ToLower(CharT c, const std::string_view& localeName = "C")
	{
		if (IsUpper(c, localeName))
		{
			const auto& facet = impl::GetFacet<std::ctype<CharT>>(localeName);
			c = facet->tolower(c);
		}
		return c;
	}

	template<class Stringish
		, class = std::enable_if_t<
		type_trait::Is_StringType_V<Stringish>
		|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)>
			&& std::is_pointer_v<Stringish>)
		>>
		USE_RESULT constexpr auto ToLower(const Stringish& input, const std::string_view& localeName = "C")
	{
		auto output = ToWriter(input);
		for (auto& c : output)
			c = ToLower(c, localeName);
		return output;
	}

	template<class CharT, size_t Size, typename = std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		&& std::is_array_v<CharT[Size]>
		>>
		USE_RESULT constexpr auto ToLower(const CharT(&input)[Size], const std::string_view& localeName = "C")
	{
		return ToLower(StringReader<std::remove_all_extents_t<CharT>>(input), localeName);
	}
	

	ENUM_CLASS(CaseSensitive, std::uint8_t, YES, NO);
}