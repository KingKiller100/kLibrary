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
		using AccuracyType = long double;

		constexpr AccuracyType GAMMA = 0.57721566490153l; // Euler's gamma constant
		constexpr AccuracyType LOG2PI_OVER_2 = 0.91893853320467l;
		constexpr AccuracyType E = 2.71828182845905l;
		constexpr AccuracyType LOG_N = 0.57721566490153l;
		constexpr AccuracyType LOG2E = 1.44269504088896l;
		constexpr AccuracyType LOG10E = 0.43429448190325l;
		constexpr AccuracyType LN2 = 0.69314718055995l;
		constexpr AccuracyType LN10 = 2.30258509299405l;
		constexpr AccuracyType PI = static_cast<AccuracyType>(M_PI);
		constexpr AccuracyType PI_OVER_2 = PI * static_cast<AccuracyType>(0.5);//1.57079632679490l;
		constexpr AccuracyType PI_OVER_4 = PI * static_cast<AccuracyType>(0.25); //0.78539816339745l;
		constexpr AccuracyType TAU = PI * static_cast<AccuracyType>(2);
		constexpr AccuracyType ROOT2 = 1.41421356237310l;
		constexpr AccuracyType SQRT_1_OVER_2 = 0.70710678118655l;
		constexpr AccuracyType GOLDEN_RATIO = 1.61803398874989l;
		constexpr AccuracyType INVERSE_GOLDEN_RATIO = static_cast<AccuracyType>(1) / GOLDEN_RATIO;

		namespace secret::helper
		{
			constexpr size_t GetCountImpl()
			{
				return 0;
			}
			template<typename T, typename ...Ts>
			constexpr size_t GetCountImpl(const T&, const Ts& ...args)
			{
				return 1 + GetCountImpl(args...);
			}
		}

		template<typename T, typename ...Ts>
		constexpr size_t Count(const T& arg, const Ts& ...args)
		{
			using namespace secret::helper;
			const auto count(GetCountImpl(arg, args...));
			return count;
		}

		template<typename T>
		USE_RESULT constexpr T RadiansToDegrees() noexcept
		{
			return (CAST(T, 360.l) / CAST(T, TAU));
		}

		template<typename T>
		USE_RESULT constexpr T DegreesToRadians() noexcept
		{
			return (CAST(T, TAU) / CAST(T, 360.l));
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
		USE_RESULT constexpr bool ApproximatelyEpsilon(T&& value) noexcept
		{
			constexpr auto epsilon = Epsilon<T>();

			return (value >= -epsilon) && (value <= epsilon);
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
			constexpr auto lb = target - epsilon;
			constexpr auto ub = target + epsilon;

			return (value >= lb) && (value <= ub);
		}

		template<class T>
		USE_RESULT constexpr bool ApproximatelyOne(T value) noexcept
		{
			return Approximately(value, One<T>());
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
