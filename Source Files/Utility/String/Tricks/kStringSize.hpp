#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString
{
	template<typename StringType
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_t<StringType>
#else
		, typename = std::enable_if_t<type_trait::Is_StringType_V<StringType>> >
#endif
		USE_RESULT constexpr size_t GetSize(const StringType & str)
	{
		return str.size();
	}

	template<typename CharT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<CharT>
#else
		, typename = std::enable_if_t<type_trait::Is_CharType_V<CharT>> >
#endif
		USE_RESULT constexpr size_t GetSize(const CharT * str)
	{
		return std::char_traits<CharT>::length(str);
	}
}