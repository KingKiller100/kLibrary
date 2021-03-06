﻿#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

#include "../../Localization/kLocale.hpp"

namespace klib::kString
{
	template<class CharT, class = std::enable_if_t<
		type_trait::Is_Char_V<CharT>
		>>
		USE_RESULT constexpr bool IsWhiteSpace(CharT c)
	{
		const auto isWhiteSpace = kLocale::Query(c, std::ctype_base::blank);
		return isWhiteSpace;
	}

	template<typename Stringish, class = std::enable_if_t<
		type_trait::Is_CString_V<Stringish>
		|| type_trait::Is_String_V<Stringish>
		>>
		USE_RESULT constexpr bool IsWhiteSpaceOrNull(const Stringish& str)
	{
		using namespace type_trait;
		if _CONSTEXPR17(Is_String_V<Stringish>)
		{
			if (str.empty())
				return true;

			for (const auto& c : str)
			{
				if (!IsWhiteSpace(c))
					return false;
			}
		}
		else
		{
			if (str == nullptr)
				return false;

			size_t index = 0;
			while (str[index] != g_NullTerminator<Simplify_t<Stringish>>)
			{
				if (!IsWhiteSpace(str[index]))
					return false;
				++index;
			}
		}

		return true;
	}
}