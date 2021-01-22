#pragma once

#include <type_traits>

namespace klib::type_trait
{
	// STRUCT TEMPLATE _Is_specialization
	template <class T, template <class...> class Template>
	constexpr bool Is_SpecializationBase = false; // true if and only if T is a specialization of Template
	template <template <class...> class Template, class... Ts>
	constexpr bool Is_SpecializationBase<Template<Ts...>, Template> = true;

	template <class T, template <class...> class Template>
	struct Is_Specialization : std::bool_constant<Is_SpecializationBase<std::remove_cv_t<T>, Template>> {};

	template <class T, template <class...> class Template>
	constexpr bool Is_Specialization_V = Is_Specialization<T, Template>::value; // true if and only if T is a specialization of Template

	// VARIABLE TEMPLATE _Is_any_of_v
	template <class T, class... Ts>
	_INLINE_VAR constexpr bool Is_It_V = // true if and only if T is in Ts
		std::disjunction_v<std::is_same<T, Ts>...>;

	template <class T, class... Ts>
	struct Is_It : std::bool_constant<Is_It_V<T, Ts...>> {};

	template<typename T>
	using Simplify_t = std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>;
}
