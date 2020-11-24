#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

#include <locale>
#include <string>

namespace klib::kString
{
	namespace secret::impl
	{
		std::locale& GetLocale();

		const std::string& GetLocaleName();
		void SetLocale( const std::string_view& localeName, std::locale::category cat = std::locale::all );

		template<typename Facet_t>
		USE_RESULT constexpr const Facet_t& GetFacet() noexcept
		{
			const auto& facet = std::use_facet<Facet_t>(GetLocale());
			return facet;
		}
	}

	template<class Char_t, class = std::enable_if_t<type_trait::Is_CharType_V<Char_t>>>
	USE_RESULT constexpr bool IsIt(Char_t c, std::ctype_base::mask mask)
	{
		const auto& facet = secret::impl::GetFacet<std::ctype<Char_t>>();
		const auto result = facet.is(mask, c);
		return result;
	}

	template<class Char_t, typename = std::enable_if_t<type_trait::Is_CharType_V<Char_t>>>
	USE_RESULT constexpr bool IsDigit(Char_t c)
	{
		const auto isDigit = IsIt(c, std::ctype_base::digit);
		return isDigit;
	}
}