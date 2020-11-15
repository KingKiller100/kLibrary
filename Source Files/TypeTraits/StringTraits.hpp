#pragma once

#include "TemplateTraits.hpp"

#include <string>
#include <type_traits>

namespace klib::type_trait
{
	template<typename T>
	struct Is_CharTypeBase : std::false_type
	{};

	template<>
	struct Is_CharTypeBase<char> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<unsigned char> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<signed char> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<wchar_t> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<char16_t> : std::true_type
	{};

	template<>
	struct Is_CharTypeBase<char32_t> : std::true_type
	{};

#ifdef __cpp_char8_t
	template<>
	struct Is_CharTypeBase<char8_t> : std::true_type
	{};
#endif

	template<typename T>
	struct Is_CharType : Is_CharTypeBase<std::remove_cv_t<T>>
	{};

	// Determines whether type is a character type
	template<typename T>
	constexpr bool Is_CharType_V = Is_CharType<T>::value;

	template<typename T>
	concept Is_Char_t = Is_CharType_V<T> == true;

	// Determines whether type is an STL string class type
	template<typename T>
	constexpr bool Is_StringType_V = 
		Is_Specialization_V<T, std::basic_string>
		|| Is_Specialization_V<T, std::basic_string_view>;
	
	template<typename T>
	struct Is_StringTypeBase : std::bool_constant<Is_StringType_V<T>>
	{};

	template<typename T>
	struct Is_StringType : Is_StringTypeBase<std::remove_cv_t<T>>
	{};
	
	template<typename T>
	concept Is_String_t = Is_StringType_V<T> == true;

	template<typename CharType, typename = std::enable_if_t<Is_CharType_V<CharType>>>
	constexpr auto s_NullTerminator = CharType();
}
