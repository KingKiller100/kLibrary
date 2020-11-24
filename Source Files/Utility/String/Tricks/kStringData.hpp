#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString
{
	template<typename StringT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_t<StringT>
#else
		, typename = std::enable_if_t<type_trait::Is_StringType_V<StringT>> >
#endif
	USE_RESULT constexpr const typename StringT::value_type* GetData(const StringT& str) noexcept
	{
		return str.data();
	}
	
	template<typename CStringT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_CString_t<StringT>
#else
		, typename = std::enable_if_t<type_trait::Is_CString_V<CStringT>> >
#endif
	USE_RESULT constexpr const ONLY_TYPE( CStringT )* GetData(const CStringT& cstr) noexcept
	{
		return cstr;
	}
	
	template<typename CharT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<StringT>
#else
		, typename = std::enable_if_t<type_trait::Is_CharType_V<CharT>> >
#endif
	USE_RESULT constexpr CharT GetData(CharT c) noexcept
	{
		return c;
	}

	
}