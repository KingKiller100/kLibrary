#pragma once
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString::format
{
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto g_PrintfSymbol = CharT('%');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto g_OpenerSymbol = CharT('{');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto g_CloserSymbol = CharT('}');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto g_SpecifierSymbol = CharT(':');
}
