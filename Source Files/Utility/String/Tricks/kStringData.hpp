#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "../../../TypeTraits/SmartPointerTraits.hpp"

namespace klib::kString
{
	template<typename StringT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_t<StringT>
#else
		, typename = std::enable_if_t<type_trait::Is_StringType_V<StringT>> >
#endif
		USE_RESULT constexpr const typename StringT::value_type * GetData(const StringT & str) noexcept
	{
		return str.data();
	}

	template<typename CStringT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_CString_t<CStringT>
#else
		, typename = std::enable_if_t<type_trait::Is_CString_V<CStringT>> >
#endif
		USE_RESULT constexpr const ONLY_TYPE(CStringT) * GetData(CStringT cstr) noexcept
	{
		using Ptr_t = CStringT;
		return (const Ptr_t)cstr;
	}

	template<typename CharT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<CharT>
#else
		, typename = std::enable_if_t<type_trait::Is_CharType_V<CharT>> >
#endif
		USE_RESULT constexpr CharT GetData(CharT c) noexcept
	{
		return c;
	}

	template<typename CharT, size_t Size
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<CharT>
#else
		, typename = std::enable_if_t<type_trait::Is_CharType_V<CharT>> >
#endif
		USE_RESULT constexpr const CharT* GetData(const CharT (&c)[Size]) noexcept
	{
		return c;
	}

	template<typename CharT, size_t Size
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<CharT>
#else
		, typename = std::enable_if_t<type_trait::Is_CharType_V<CharT>> >
#endif
		USE_RESULT constexpr const CharT* GetData(CharT (&c)[Size]) noexcept
	{
		return (const CharT*)c;
	}

	template<typename SmartPtr_t
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_SmartPtr_t<SmartPtr_t>
#else
		, typename = std::enable_if_t<type_trait::Is_SmartPtr_V<SmartPtr_t>> >
#endif
		USE_RESULT constexpr const typename SmartPtr_t::element_type * GetData(const SmartPtr_t & sptr) noexcept
	{
		return sptr.get();
	}

}