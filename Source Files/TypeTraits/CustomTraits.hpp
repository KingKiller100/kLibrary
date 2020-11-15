#pragma once
#include "StringTraits.hpp"

#include <type_traits>

namespace klib::type_trait
{
	template<typename T>
	constexpr bool Is_CustomType_V =
		!type_trait::Is_CharType_V<T>
		&& !std::is_arithmetic_v<T>
		&& !type_trait::Is_StringType_V<T>
		&& !std::is_pointer_v<T>
		&& !std::is_array_v<T>;

	template<typename T>
	struct Is_CustomBase : std::bool_constant<Is_CustomType_V<T>>
	{};

	template<typename T>
	struct Is_Custom : Is_CustomBase<std::remove_cv_t<T>>
	{};
	
}
