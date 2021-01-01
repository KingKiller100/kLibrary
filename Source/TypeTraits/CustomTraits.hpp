#pragma once
#include "StringTraits.hpp"
#include "SmartPointerTraits.hpp"

#include <type_traits>

namespace klib::type_trait
{
	template<typename T>
	struct Is_CustomBase : std::bool_constant<
		!std::is_fundamental_v<T>
		&& !Is_String_V<T>
		&& !Is_SmartPtr_V<T>
		&& !std::is_pointer_v<T>
		&& !std::is_array_v<T>
	>
	{};

	template<typename T>
	struct Is_Custom : Is_CustomBase<std::remove_cv_t<T>>
	{};

	/* Only TRUE if the given type is NEITHER:
	 * A raw pointer
	 * A C array
	 * A fundamental type
	 * A STL string[_view] type
	 * A STL smart pointer
	*/
	template<typename T>
	constexpr bool Is_CustomType_V = Is_Custom<T>::value;
}
