#pragma once

#include "TemplateTraits.hpp"
#include "CharacterTraits.hpp"

#include <string>
#include <type_traits>


namespace klib::type_trait
{	
	// Determines whether type is an STL string class type
	template<typename T>
	struct Is_StringBase : std::bool_constant<
		Is_Specialization_V< T, std::basic_string>
		|| Is_Specialization_V<T, std::basic_string_view>
	>
	{
		using Type = T;
	};
	
	// Determines whether type is an STL string class type
	template<typename T>
	struct Is_String : Is_StringBase<std::remove_cv_t<T>> {};

	
	// Determines whether type is an C character ptr type
	template<typename T>
	struct Is_CStringBase : std::bool_constant<
		std::is_pointer_v<T> && Is_Char_V<std::remove_pointer_t<T>>
	>
	{
		using Char_t = std::remove_cv_t<std::remove_pointer_t<T>>;
	};

	// Determines whether type is an C character ptr type
	template<typename T>
	struct Is_CString : Is_CStringBase<std::decay_t<T>> {};


	// Determines whether type is an STL string or C character ptr type
	template<typename T>
	struct Is_Stringish : std::bool_constant<
		Is_CString<T>::value || Is_String<T>::value
	>
	{};

	// Determines whether type is an STL string /class type
	template<typename T>
	constexpr bool Is_String_V = Is_String<T>::value;

	// Determines whether type is an C character ptr type
	template<typename T>
	constexpr bool Is_CString_V = Is_CString<T>::value;
	
	// Determines whether type is an STL string or C character ptr type
	template<typename T>
	constexpr bool Is_Stringish_V = Is_Stringish<T>::value;

#if MSVC_PLATFORM_TOOLSET > 141
	template<typename T>
	concept Is_String_t = Is_String_V<T> == true;
	
	template<typename T>
	concept Is_CString_t = Is_CString_V<T> == true;
	
	template<typename T>
	concept Is_Stringish_t = Is_Stringish_V<T> == true;
#endif
	
	template<class StringT, class = std::enable_if_t<Is_String_V<StringT>>>
	constexpr auto g_NoPos = StringT::npos;
	
	template<class CharT>
	constexpr const CharT * EmptyString() { return nullptr; }

	template<>
	constexpr const char * EmptyString<char>() { return ""; }

	template<>
	constexpr const wchar_t * EmptyString<wchar_t>() { return L""; }

	template<>
	constexpr const char16_t * EmptyString<char16_t>() { return u""; }

	template<>
	constexpr const char32_t * EmptyString<char32_t>() { return U""; }

#if __cpp_char8_t
	template<>
	constexpr const char8_t * EmptyString<char8_t>() { return u8""; }
#endif

}
