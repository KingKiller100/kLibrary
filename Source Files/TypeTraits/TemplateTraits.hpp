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
}
