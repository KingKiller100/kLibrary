﻿#pragma once
#include "../../../TypeTraits/StringTraits.hpp"
#include "kStringFind.hpp"

namespace klib::kString
{
	template<typename StringType, typename Stringish
#if MSVC_PLATFORM_TOOLSET >= 142
			> requires type_trait::Is_String_c<StringType>
		&& (type_trait::Is_String_c<Stringish>     // STL string
			|| type_trait::Is_CString_c<Stringish> // C style string
			|| type_trait::Is_Char_c<Stringish>)   // Just a character
#else
		, typename = std::enable_if_t<
		type_trait::Is_String_V<StringType>
		&& type_trait::Is_String_V<Stringish>   // STL string
		|| type_trait::Is_CString_V<Stringish>  // C style string
		|| type_trait::Is_Char_V<Stringish> // Just a character
		>>
#endif
		constexpr bool Remove(StringType & str, const Stringish & search, size_t offset = 0)
	{
		const auto data = str.data();
		
		auto pos = Find(data, search, offset);
		if (pos == StringType::npos)
			return false;

		while (pos != StringType::npos)
		{
			const auto endPos = str.find_first_not_of(search, pos);
			str.erase(pos, endPos - pos);
			pos = Find(data, search, offset);
		}

		return true;
	}
}