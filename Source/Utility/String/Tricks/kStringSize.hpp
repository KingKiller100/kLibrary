#pragma once
#include "../../../HelperMacros.hpp"

#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString
{
	template<typename StringType
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_c<StringType>
#else
		, typename = std::enable_if_t<type_trait::Is_String_V<StringType>> >
#endif
		USE_RESULT constexpr size_t GetSize(const StringType & str)
	{
		return str.size();
	}

	template<typename Char_t
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_c<Char_t>
#else
		, typename = std::enable_if_t<type_trait::Is_Char_V<Char_t>> >
#endif
		USE_RESULT constexpr size_t GetSize(const Char_t * str)
	{
		return type_trait::Traits<Char_t>::Length(str);
	}

	template<typename Char_t
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_c<Char_t>
#else
		, typename = std::enable_if_t<type_trait::Is_Char_V<Char_t>> >
#endif
		USE_RESULT constexpr size_t GetSize(Char_t * str)
	{
		return GetSize((const Char_t*)str);
	}

	template<typename Char_t, size_t Size
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_c<Char_t>
#else
		, typename = std::enable_if_t<type_trait::Is_Char_V<Char_t>> >
#endif
		USE_RESULT constexpr size_t GetSize(const Char_t(&)[Size])
	{
		return Size;
	}
}
