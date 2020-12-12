#pragma once

#include "TemplateTraits.hpp"
#include "CharacterTraits.hpp"

#include <string>
#include <type_traits>


namespace klib::type_trait
{	
	// Determines whether type is an STL string class type
	template<typename T>
	struct Is_StringTypeBase : std::bool_constant<
		Is_Specialization_V< T, std::basic_string>
		|| Is_Specialization_V<T, std::basic_string_view>
	>
	{
		using String_t = T;
	};

	// Determines whether type is an STL string class type
	template<typename T>
	struct Is_StringType : Is_StringTypeBase<std::remove_cv_t<T>> {};

	// Determines whether type is an STL string class type
	template<typename T>
	constexpr bool Is_StringType_V = Is_StringType<T>::value;

	
	// Determines whether type is an C character ptr type
	template<typename T>
	struct Is_CStringBase : std::bool_constant<
		std::is_pointer_v<T> && Is_CharType_V<std::remove_pointer_t<T>>
	>
	{
		using Char_t = std::remove_cv_t<std::remove_pointer_t<T>>;
	};

	// Determines whether type is an C character ptr type
	template<typename T>
	struct Is_CString : Is_CStringBase<std::decay_t<T>> {};

	// Determines whether type is an C character ptr type
	template<typename T>
	constexpr bool Is_CString_V = Is_CString<T>::value;

	template<typename T>
	constexpr bool Is_Stringish_V = (Is_CString_V<T> || Is_StringType_V<T>);

#if MSVC_PLATFORM_TOOLSET > 141
	template<typename T>
	concept Is_String_t = Is_StringType_V<T> == true;
	
	template<typename T>
	concept Is_CString_t = Is_CString_V<T> == true;
#endif
	
	template<class StringT, class = std::enable_if_t<Is_StringType_V<StringT>>>
	constexpr auto g_NoPos = StringT::npos;
}
