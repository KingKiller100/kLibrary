#pragma once

#include <type_traits>

namespace klib::type_trait
{
	// STRUCT TEMPLATE _Is_specialization
	template <class T, template <class...> class Template>
	constexpr bool Is_Specialization_V = false; // true if and only if T is a specialization of Template
	template <template <class...> class Template, class... Ts>
	constexpr bool Is_Specialization_V<Template<Ts...>, Template> = true;

	template <class _Type, template <class...> class _Template>
	struct Is_Specialization : std::bool_constant<Is_Specialization_V<_Type, _Template>> {};


	// VARIABLE TEMPLATE _Is_any_of_v
	template <class T, class... Ts>
	_INLINE_VAR constexpr bool Is_It_V = // true if and only if T is in Ts
		std::disjunction_v<std::is_same<T, Ts>...>;

	template <class T, class... Ts>
	struct Is_It : std::bool_constant<Is_It_V<T, Ts...>> {};
	
}
