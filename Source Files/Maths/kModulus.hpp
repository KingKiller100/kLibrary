#pragma once
#include "../HelperMacros.hpp"
#include "kMathsConstants.hpp"
#include <xtr1common>

namespace kmaths
{
	template <typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T FloatRemainder(T num, T base) noexcept
	{
#if MSVC_PLATFORM_TOOLSET > 142
		return std::fmod(num, base);
#else
		const auto b = CAST(constants::Accuracy_t, base);
		const auto n = CAST(constants::Accuracy_t, num);

		const auto one_over_base = constants::OneOver<constants::Accuracy_t>(b);
		const auto num_over_base = n * one_over_base;
		const auto int_n_over_b = CAST(BigInt_t, num_over_base);

		if (num_over_base == int_n_over_b)
			return constants::Zero<T>();

		const auto closestMultiplier = int_n_over_b * b;
		const auto rem = n - closestMultiplier;

		return CAST(T, rem);
#endif
	}

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Modulus(T num, T base) noexcept
	{
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
		{
			const auto mod = (num < 0)
				? FloatRemainder(num, base) + base
				: FloatRemainder(num, base);
			return mod;
		}
		else
		{
			const T rem = num % base;
			if _CONSTEXPR_IF(-1 % 2 == 1)
			{
				return rem;
			}
			else
			{
				const auto mod = rem < 0 ? rem + base : rem;
				return mod;
			}
		}
	}
}
