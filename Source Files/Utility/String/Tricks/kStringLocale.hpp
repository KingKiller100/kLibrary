#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

#include "locale"

namespace klib::kString
{
	namespace impl
	{
		template<typename Facet_t>
		USE_RESULT constexpr const Facet_t& GetFacet(const std::string_view& localeName = "C") noexcept
		{
			const std::locale locale(localeName.data());
			const auto& facet = 
				std::use_facet<std::ctype<typename Facet_t::char_type>>(locale);
			return facet;
		}
	}
	
	template<class CharT, class = std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr bool IsIt(CharT c, std::ctype_base::mask mask, const std::string_view& localeName = "C")
	{
		const auto& facet = impl::GetFacet<std::ctype<CharT>>(localeName);
		const auto result = facet.is(mask, c);
		return result;
	}

	template<class Char_t, typename = std::enable_if_t<type_trait::Is_CharType_V<Char_t>>>
	USE_RESULT constexpr bool IsDigit(Char_t c, const std::string_view& localeName = "C")
	{
		const auto isDigit = IsIt(c, std::ctype_base::digit, localeName);
		return isDigit;
	}
}