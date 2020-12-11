#pragma once
#include "../../HelperMacros.hpp"
#include "../../TypeTraits/CharacterTraits.hpp"

#include <locale>
#include <string>

namespace klib::kLocale
{
	std::locale& GetLocale();
	const std::string& GetLocaleName();
	void SetLocale(const std::string_view& localeName, std::locale::category cat = std::locale::all);

	template<typename Facet_t>
	USE_RESULT constexpr const Facet_t& GetFacet() noexcept
	{
		const auto& facet = std::use_facet<Facet_t>(GetLocale());
		return facet;
	}

	template<class Char_t, typename Facet_t = std::ctype<Char_t>, class = std::enable_if_t<type_trait::Is_CharType_V<Char_t>>>
	USE_RESULT constexpr bool Query(Char_t c, std::ctype_base::mask mask)
	{
		const auto& facet = GetFacet<Facet_t>();
		const auto result = facet.is(mask, c);
		return result;
	}
}
