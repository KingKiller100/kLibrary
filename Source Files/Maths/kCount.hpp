#pragma once
#include "../HelperMacros.hpp"
#include "kMathsFundamentals.hpp"

namespace kmaths
{
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr size_t CountIntegerDigits(T x) noexcept
	{
		constexpr T ten = CAST(T, 10);
		size_t count = 1;
		while (ten <= Abs(x))
		{
			x /= ten;
			count++;
		}
		return count;
	}
}
