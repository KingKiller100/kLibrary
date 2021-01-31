#pragma once
#include "kMathsConstants.hpp"
#include "kMathsEpsilon.hpp"
#include "kMathsTheta.hpp"

#include "../HelperMacros.hpp"

namespace kmaths
{
	template<typename T>
	USE_RESULT constexpr T ToDegrees(const T radians) noexcept
	{
		using namespace constants;
		constexpr auto convertR2D = RadsToDegs<T>;
		const T value =  radians * convertR2D;
		return value;
	}

	template<typename T>
	USE_RESULT constexpr T ToRadians(const T degrees) noexcept
	{
		using namespace constants;
		constexpr auto convertD2R = DegsToRads<Accuracy_t>;
		const T value = static_cast<T>(degrees * convertD2R);
		return value;
	}

	template<typename T>
	USE_RESULT constexpr T ConvertAngle(T angle, Theta theta) noexcept
	{
		return (theta == Theta::RADIANS) ? ToDegrees(angle) : ToRadians(angle);
	}
	
	namespace secret::impl
	{
		constexpr size_t epsilon_magnitude = 2;

		// https://stackoverflow.com/questions/34703147/sine-function-without-any-library/34703167
		template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
		USE_RESULT constexpr T SineImpl(T x, const size_t n) noexcept
		{
			constexpr auto one = constants::One<constants::Accuracy_t>;
			constexpr auto two = constants::Two<T>;
			constexpr auto tau = constants::Tau<T>;

			x = Modulus<T>(x, tau);

			const auto negSquare = CAST(constants::Accuracy_t, -x * x);

			auto t = CAST(constants::Accuracy_t, x);
			auto sine = t;
			for (size_t i = 1; i < n; ++i)
			{
				const constants::Accuracy_t xn = negSquare / ((two * i + one) * (two * i));
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
		using namespace secret::impl;
		
		const auto xf = static_cast<ClosestFloat_t<T>>(x);
		const auto sine_x = HandleEpsilon<ClosestFloat_t<T>>(SineImpl<ClosestFloat_t<T>>(xf, n), epsilon_magnitude);
		return static_cast<T>(sine_x);
	}

	// Uses Taylor series to iterate through to get the better approximation of sine(x)
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Cosine(T x, const size_t n = 250) noexcept
	{
		using namespace secret::impl;

		const auto xf = static_cast<ClosestFloat_t<T>>(x) + constants::Pi_Over_2<ClosestFloat_t<T>>;
		const auto cosine_x = HandleEpsilon<ClosestFloat_t<T>>(SineImpl<ClosestFloat_t<T>>(xf, n), epsilon_magnitude);
		return static_cast<T>(cosine_x);
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
