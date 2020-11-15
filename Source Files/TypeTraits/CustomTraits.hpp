#pragma once
#include "StringTraits.hpp"

#include <type_traits>

namespace klib::type_trait
{
	template<typename T>
	struct Is_CustomBase : std::bool_constant<
		!std::is_arithmetic_v<T>
		&& !Is_StringType_V<T>
		&& !std::is_pointer_v<T>
		&& !std::is_array_v<T>
	>
	{};

	template<typename T>
	struct Is_Custom : Is_CustomBase<std::remove_cv_t<T>>
	{};

	/* Checks if the given type is NEITHER:
	 * A raw pointer
	 * A C array
	 * An arithmetic type
	 * A STL string[_view] type
	*/
	template<typename T>
	constexpr bool Is_CustomType_V = Is_Custom<T>::value;
}
