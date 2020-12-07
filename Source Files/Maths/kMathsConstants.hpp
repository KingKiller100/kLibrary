#pragma once

#include "../HelperMacros.hpp"

#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#include <type_traits>
#include <limits>


namespace kmaths
{
	namespace constants
	{
		using Accuracy_t = long double;
		template<typename T = Accuracy_t> constexpr T GAMMA                = static_cast<T>(0.57721566490153l); // Euler's gamma constant
		template<typename T = Accuracy_t> constexpr T LOG2PI_OVER_2        = static_cast<T>(0.91893853320467l);
		template<typename T = Accuracy_t> constexpr T E                    = static_cast<T>(2.71828182845905l);
		template<typename T = Accuracy_t> constexpr T LOG_N                = static_cast<T>(0.57721566490153l);
		template<typename T = Accuracy_t> constexpr T LOG2E                = static_cast<T>(1.44269504088896l);
		template<typename T = Accuracy_t> constexpr T LOG10E               = static_cast<T>(0.43429448190325l);
		template<typename T = Accuracy_t> constexpr T LN2                  = static_cast<T>(0.69314718055995l);
		template<typename T = Accuracy_t> constexpr T LN10                 = static_cast<T>(2.30258509299405l);
		template<typename T = Accuracy_t> constexpr T PI                   = static_cast<T>(M_PI);
		template<typename T = Accuracy_t> constexpr T PI_OVER_2            = static_cast<T>(PI<Accuracy_t> * static_cast<Accuracy_t>(0.5l)); // 1.57079632679490l;
		template<typename T = Accuracy_t> constexpr T PI_OVER_4            = static_cast<T>(PI<Accuracy_t> * static_cast<Accuracy_t>(0.25l)); // 0.78539816339745l;
		template<typename T = Accuracy_t> constexpr T TAU                  = static_cast<T>(PI<Accuracy_t> * static_cast<Accuracy_t>(2.l));
		template<typename T = Accuracy_t> constexpr T ROOT2                = static_cast<T>(1.41421356237310l);
		template<typename T = Accuracy_t> constexpr T SQRT_1_OVER_2        = static_cast<T>(0.70710678118655l);
		template<typename T = Accuracy_t> constexpr T GOLDEN_RATIO         = static_cast<T>(1.61803398874989l);
		template<typename T = Accuracy_t> constexpr T INVERSE_GOLDEN_RATIO = static_cast<T>(static_cast<Accuracy_t>(1) / GOLDEN_RATIO);

		template<typename T>
		USE_RESULT constexpr T RadiansToDegrees() noexcept
		{
			return (CAST(T, 360.l) / TAU<T>);
		}

		template<typename T>
		USE_RESULT constexpr T DegreesToRadians() noexcept
		{
			return (TAU<T> / CAST(T, 360.l));
		}

		template<class T>
		USE_RESULT constexpr T Infinity() noexcept
		{
			return std::numeric_limits<T>::infinity();
		}

		template<class T>
		USE_RESULT constexpr T Epsilon() noexcept
		{
			return std::numeric_limits<T>::epsilon();
		}

		template<class T>
		USE_RESULT constexpr T Zero() noexcept
		{
			return T();
		}

		template<class T>
		USE_RESULT constexpr T One() noexcept
		{
			return T(1);
		}

		template<class T>
		USE_RESULT constexpr T One(T&&) noexcept
		{
			return One<ONLY_TYPE(T)>();
		}

		template<class T1, typename T2>
		USE_RESULT constexpr bool Approximately(T1 value, T2 target) noexcept
		{
			constexpr auto epsilon = Epsilon<T1>();
			const auto lb = static_cast<T1>(target) - epsilon;
			const auto ub = static_cast<T1>(target) + epsilon;

			return (value >= lb) && (value <= ub);
		}

		template<class T>
		USE_RESULT constexpr bool ApproximatelyOne(T value) noexcept
		{
			return Approximately(value, One<T>());
		}

		template<class T>
		USE_RESULT constexpr bool ApproximatelyZero(T value) noexcept
		{
			return Approximately(value, Zero<T>());
		}

		template<class T>
		USE_RESULT constexpr T MinusOne() noexcept
		{
			return T(-1);
		}

		template<class T>
		USE_RESULT constexpr T MinusOne(T&&) noexcept
		{
			return MinusOne<ONLY_TYPE(T)>();
		}

		template<class T>
		USE_RESULT constexpr T Two() noexcept
		{
			return T(2);
		}

		template<class T>
		USE_RESULT constexpr T Two(T&&) noexcept
		{
			return Two<ONLY_TYPE(T)>();
		}

		template<class T>
		USE_RESULT constexpr T ZeroPointOne() noexcept
		{
			return T(0.1);
		}

		template<class T>
		USE_RESULT constexpr T ZeroPointOne(T&&) noexcept
		{
			return ZeroPointOne<ONLY_TYPE(T)>();
		}

		template<class T>
		USE_RESULT constexpr T ZeroPointFive() noexcept
		{
			return T(0.5);
		}

		template<class T>
		USE_RESULT constexpr T ZeroPointFive(T&&) noexcept
		{
			return ZeroPointFive<T>();
		}

		template<class T>
		USE_RESULT constexpr T OnePointFive() noexcept
		{
			return T(1.5);
		}

		template<class T>
		USE_RESULT constexpr T OnePointFive(T&&) noexcept
		{
			return OnePointFive<T>();
		}

		template<class ReturnType, class T>
		USE_RESULT constexpr ReturnType OneOver(T value) noexcept
		{
			return One<ReturnType>() / value;
		}

		template<class ReturnType, class T, class U>
		USE_RESULT constexpr ReturnType Divide(T&& x, U&& y) noexcept
		{
			return ReturnType(std::forward<T>(x)) / std::forward<U>(y);
		}
	}

	using namespace constants;
}
