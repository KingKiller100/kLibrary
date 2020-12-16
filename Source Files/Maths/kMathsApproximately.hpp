#pragma once

#include "kMathsConstants.hpp"

namespace kmaths
{
	template<class T1, typename T2>
	USE_RESULT constexpr bool Approximately(T1 value, T2 target) noexcept
	{
		constexpr auto epsilon = constants::Epsilon<T1>();
		const auto lb = static_cast<T1>(target) - epsilon;
		const auto ub = static_cast<T1>(target) + epsilon;

		return (value >= lb) && (value <= ub);
	}

	template<class T>
	USE_RESULT constexpr bool ApproximatelyOne(T value) noexcept
	{
		return Approximately(value, constants::One<T>());
	}

	template<class T>
	USE_RESULT constexpr bool ApproximatelyZero(T value) noexcept
	{
		return Approximately(value, constants::Zero<T>());
	}
}