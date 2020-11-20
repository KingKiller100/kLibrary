#pragma once
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString::format
{
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto printfSymbol = CharT('%');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto openerSymbol = CharT('{');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto closerSymbol = CharT('}');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto specifierSymbol = CharT(':');
}
