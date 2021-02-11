#pragma once

#include "TemplateTraits.hpp"

#include <type_traits>

#include <vector>
#include <set>
#include <deque>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace klib::type_trait
{
	template<typename T>
	struct Is_STL_Container : std::bool_constant<
		Is_Specialization_V<T, std::vector>
		|| Is_Specialization_V<T, std::map>
		|| Is_Specialization_V<T, std::set>
		|| Is_Specialization_V<T, std::unordered_set>
		|| Is_Specialization_V<T, std::unordered_multimap>
		|| Is_Specialization_V<T, std::unordered_map>
		|| Is_Specialization_V<T, std::deque>
		|| Is_Specialization_V<T, std::stack>
		|| Is_Specialization_V<T, std::queue>
		|| Is_Specialization_V<T, std::priority_queue>
		|| Is_Specialization_V<T, std::list>
	>
	{};

	template<typename T>
	constexpr bool Is_STL_V = Is_STL_Container<T>::value;
	
}