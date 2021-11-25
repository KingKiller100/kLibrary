#pragma once
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString::format
{
	template<class CharT, class = std::enable_if_t<type_trait::Is_Char_V<CharT>>>
	constexpr auto g_PrintfSymbol = CharT('%');
	template<class CharT, class = std::enable_if_t<type_trait::Is_Char_V<CharT>>>
	constexpr auto g_ToStringOpenerSymbol = CharT('{');
	template<class CharT, class = std::enable_if_t<type_trait::Is_Char_V<CharT>>>
	constexpr auto g_ToStringCloserSymbol = CharT('}');
	template<class CharT, class = std::enable_if_t<type_trait::Is_Char_V<CharT>>>
	constexpr auto g_ToStringSpecifierSymbol = CharT(':');
}
