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

		// Fundamentals
		namespace fundamentals
		{
			template<typename T> constexpr T EulerGamma = static_cast<T>(0.57721566490153l); // Euler's gamma constant
			template<typename T> constexpr T Log_Tau_Over_2 = static_cast<T>(0.91893853320467l);
			template<typename T> constexpr T E = static_cast<T>(2.71828182845905l);
			template<typename T> constexpr T Log_N = static_cast<T>(0.57721566490153l);
			template<typename T> constexpr T Log_B2_E = static_cast<T>(1.44269504088896l);
			template<typename T> constexpr T Log_B10_E = static_cast<T>(0.43429448190325l);
			template<typename T> constexpr T Ln2 = static_cast<T>(0.69314718055995l);
			template<typename T> constexpr T Ln10 = static_cast<T>(2.30258509299405l);
			template<typename T> constexpr T Pi = static_cast<T>(M_PI);
			template<typename T> constexpr T Pi_Over_2 = static_cast<T>(Pi<Accuracy_t> * static_cast<Accuracy_t>(0.5l)); // 1.57079632679490l;
			template<typename T> constexpr T Pi_Over_4 = static_cast<T>(Pi<Accuracy_t> * static_cast<Accuracy_t>(0.25l)); // 0.78539816339745l;
			template<typename T> constexpr T Tau = static_cast<T>(Pi<Accuracy_t> *static_cast<Accuracy_t>(2.l));
			template<typename T> constexpr T Root2 = static_cast<T>(1.41421356237310l);
			template<typename T> constexpr T Sqrt_1_Over_2 = static_cast<T>(0.70710678118655l);
			template<typename T> constexpr T GoldenRatio = static_cast<T>(1.61803398874989l);
			template<typename T> constexpr T InverseGoldenRatio = static_cast<T>(static_cast<Accuracy_t>(1) / GoldenRatio<Accuracy_t>);

			template<typename T> constexpr T RadsToDegs = static_cast<T>(360.l) / Tau<T>;
			template<typename T> constexpr T DegsToRads = Tau<T> / static_cast<T>(360.l);
		}

		// Numbers
		namespace numbers
		{
			template<typename T> constexpr T Infinity = std::numeric_limits<T>::infinity();
			template<typename T> constexpr T Epsilon = std::numeric_limits<T>::epsilon();

			template<typename T> constexpr T MinusOne = T(-1);
			template<typename T> constexpr T Zero = T(0);
			template<typename T> constexpr T ZeroPointOne = T(0.1l);
			template<typename T> constexpr T ZeroPointFive = T(0.5l);
			template<typename T> constexpr T One = T(1);
			template<typename T> constexpr T Two = T(2);
		}

		namespace operators
		{
			template<class ReturnType, class T>
			USE_RESULT constexpr ReturnType OneOver(T value) noexcept
			{
				return numbers::One<ReturnType> / value;
			}

			template<class ReturnType, class T, class U>
			USE_RESULT constexpr ReturnType Divide(T&& x, U&& y) noexcept
			{
				return ReturnType(std::forward<T>(x)) / std::forward<U>(y);
			}
		}

		using namespace fundamentals;
		using namespace numbers;
		using namespace operators;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace constants;
#endif
}
