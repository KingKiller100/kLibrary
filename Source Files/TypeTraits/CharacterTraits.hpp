#pragma once
#include "TemplateTraits.hpp"

namespace klib::type_trait
{
	template<typename T>
	struct Is_CharTypeBase : std::bool_constant<Is_It_V<T,
		char, unsigned char, signed char, wchar_t, char16_t, char32_t
#ifdef __cpp_char8_t
		, char8_t
#endif
		>>
	{};

	template<typename T>
	struct Is_CharType : Is_CharTypeBase<std::remove_cv_t<T>>
	{};

	// Determines whether type is a character type
	template<typename T>
	constexpr bool Is_CharType_V = Is_CharType<T>::value;

#if MSVC_PLATFORM_TOOLSET > 141
	template<typename T>
	concept Is_Char_t = Is_CharType_V<T> == true;
#endif
	

	template<typename CharType, typename = std::enable_if_t<Is_CharType_V<CharType>>>
	constexpr auto s_NullTerminator = CharType();

}