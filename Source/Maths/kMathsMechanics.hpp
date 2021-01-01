#pragma once
#include "../HelperMacros.hpp"

namespace kmaths
{
	template <typename T>
	USE_RESULT constexpr T Lerp(T a, T b, T t) noexcept
	{
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT T GetAccelerationOverTime(T initialVelocity, T distance) noexcept
	{
		const T acceleration = (0 - (initialVelocity * initialVelocity)) / (2 * distance);
		return acceleration;
	}
}
