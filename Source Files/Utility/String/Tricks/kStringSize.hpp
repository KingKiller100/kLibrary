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

	template<typename Char_t
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<Char_t>
#else
		, typename = std::enable_if_t<type_trait::Is_CharType_V<Char_t>> >
#endif
		USE_RESULT constexpr size_t GetSize(const Char_t* str)
	{
		size_t count(0);
		while ( *str != type_trait::g_NullTerminator<Char_t> )
		{
			++str;
			++count;
		}
		return count;
	}

	template<typename Char_t
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<Char_t>
#else
		, typename = std::enable_if_t<type_trait::Is_CharType_V<Char_t>> >
#endif
		USE_RESULT constexpr size_t GetSize(Char_t* str)
	{
		size_t count(0);
		while ( *str != type_trait::g_NullTerminator<Char_t> )
		{
			++str;
			++count;
		}
		return count;
	}
}