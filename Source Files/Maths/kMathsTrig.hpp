#pragma once
#include "Constants.hpp"
#include "../HelperMacros.hpp"

namespace kmaths
{
	template<typename T>
	USE_RESULT constexpr T ToDegrees(const T radians) noexcept
	{
		constexpr auto convertR2D = constants::RadiansToDegrees<T>();
		return CAST(T, radians * convertR2D);
	}

	template<typename T>
	USE_RESULT constexpr T ToRadians(const T degrees) noexcept
	{
		constexpr auto convertD2R = constants::DegreesToRadians<constants::Accuracy_t>();
		return CAST(T, degrees * convertD2R);
	}

	namespace secret::impl
	{
		// https://stackoverflow.com/questions/34703147/sine-function-without-any-library/34703167
		template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
		USE_RESULT constexpr T SineImpl(T x, const size_t n) noexcept
		{
			constexpr auto one = constants::One<constants::Accuracy_t>();
			constexpr auto two = constants::Two<T>();
			constexpr auto tau = CAST(T, constants::TAU);

			x = Modulus<T>(x, tau);

			const auto square = CAST(constants::Accuracy_t, -x * x);

			auto t = CAST(constants::Accuracy_t, x);
			auto sine = t;
			for (size_t a = 1; a < n; ++a)
			{
				const constants::Accuracy_t xn = square / ((two * a + one) * (two * a));
				t *= xn;
				sine += t;
			}
			return CAST(T, sine);
		}
	}

	// Uses Taylor series to iterate through to get the better approximation of sine(x)
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Sine(T x, const size_t n = 250) noexcept
	{
		using namespace kmaths::secret::impl;
		constexpr constants::Accuracy_t epsilon_magnitude = 2;
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
			return HandleEpsilon(SineImpl<T>(x, n), epsilon_magnitude);
		else
			return CAST(T, HandleEpsilon<float>(SineImpl<float>(CAST(float, x), n), epsilon_magnitude));
	}

	// Uses Taylor series to iterate through to get the better approximation of sine(x)
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Cosine(T x, const size_t n = 250) noexcept
	{
		using namespace kmaths::secret::impl;
		constexpr constants::Accuracy_t epsilon_magnitude = 2;
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
		{
			const auto xf = x + CAST(T, constants::PI_OVER_2);
			return HandleEpsilon(SineImpl<T>(xf, n), epsilon_magnitude);
		}
		else
		{
			const auto xf = CAST(float, x) + CAST(float, constants::PI_OVER_2);
			return CAST(T, HandleEpsilon<float>(SineImpl<float>(xf, n), epsilon_magnitude));
		}
	}

	// Uses Taylor series to iterate through to get the better approximation of sine(x)
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Tan(T x, const size_t n = 250)
	{
		const auto sine = Sine(x, n);
		const auto cosine = Cosine(x, n);
		return (sine / cosine);
	}
}
