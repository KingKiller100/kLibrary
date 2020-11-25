#pragma once
#include "../../../TypeTraits/StringTraits.hpp"
#include "kStringFind.hpp"

namespace klib::kString
{
	template<typename StringType, typename Stringish
#if MSVC_PLATFORM_TOOLSET >= 142
			> requires type_trait::Is_String_t<StringType>
		&& (type_trait::Is_String_t<Stringish>     // STL string
			|| type_trait::Is_CString_t<Stringish> // C style string
			|| type_trait::Is_Char_t<Stringish>)   // Just a character
#else
		, typename = std::enable_if_t<
		type_trait::Is_StringType_V<StringType>
		&& type_trait::Is_StringType_V<Stringish>   // STL string
		|| type_trait::Is_CString_V<Stringish>  // C style string
		|| type_trait::Is_CharType_V<Stringish> // Just a character
		>>
#endif
		constexpr bool Remove(StringType & str, const Stringish & search, size_t offset = 0)
	{
		auto pos = Find(str.data(), search, offset);
		if (pos == StringType::npos)
			return false;

		while (pos != StringType::npos)
		{
			const auto endPos = str.find_first_not_of(search, pos);
			str.erase(pos, endPos - pos);
			pos = Find(str.data(), search, offset);
		}

		return true;
	}
}