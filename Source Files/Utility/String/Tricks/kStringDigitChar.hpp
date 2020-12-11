#pragma once
#include "../../../TypeTraits/CharacterTraits.hpp"
#include "../../Locale/kLocale.hpp"

namespace klib::kString
{
	template<class Char_t, typename = std::enable_if_t<type_trait::Is_CharType_V<Char_t>>>
	USE_RESULT constexpr bool IsDigit(Char_t c)
	{
		const auto isDigit = kLocale::Query(c, std::ctype_base::digit);
		return isDigit;
	}
}
