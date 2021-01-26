#pragma once
#include "kMathsConstants.hpp"
#include "kMathsFloat.hpp"
#include "kMathsFundamentals.hpp"

namespace kmaths
{
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T HandleEpsilon(T value, const constants::Accuracy_t magnitude = 1.l) noexcept
	{
		if (value == 0)
			return 0;

		const auto epsilon = CAST(T, constants::Epsilon<T> * magnitude);
		const auto integer = Floor(value);
		const auto decimals = GetDecimals(value);
		return (Abs(decimals) <= epsilon) ? integer : value;
	}
}
