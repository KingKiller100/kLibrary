#pragma once
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString::format
{
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto s_PrintfSymbol = CharT('%');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto s_OpenerSymbol = CharT('{');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto s_CloserSymbol = CharT('}');
	template<class CharT, class = std::enable_if_t<type_trait::Is_CharType_V<CharT>>>
	constexpr auto s_SpecifierSymbol = CharT(':');
}
