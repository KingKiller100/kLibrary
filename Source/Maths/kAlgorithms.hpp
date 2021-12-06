#pragma once

#include "kFraction.hpp"
#include "kMathsConstants.hpp"
#include "Length_Type.hpp"
#include "kMathsFundamentals.hpp"
#include "kBinarySearch.hpp"
#include "kPowerOf.hpp"
#include "kMathsCount.hpp"
#include "kMathsTrig.hpp"
#include "kMathsEpsilon.hpp"
#include "kMathsApproximately.hpp"
#include "kMathsFloat.hpp"
#include "kModulus.hpp"
#include "kRound.hpp"

#include "../HelperMacros.hpp"
#include "../Utility/Debug/Exceptions/MathsExceptions.hpp"

#	include <cmath>

#include <cstdint>
#include <type_traits>

namespace kmaths
{
	namespace secret::impl
	{
		// Bakhshali Method
		template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
		USE_RESULT constexpr T SqrtImpl(T square)
		{
#if MSVC_PLATFORM_TOOLSET > 142
			return CAST(T, sqrt(square));
#else
			using namespace constants;

			auto maxIterations = std::numeric_limits<T>::max_digits10;

			if (IsNegative(square))
				throw klib::kDebug::NoRealRootError(square, 2);

			if (square == constants::ZeroPointFive<T>)
				return constants::Sqrt_1_Over_2<T>;

			if (square == 2)
				return constants::Root2<T>;

			const auto chooseStartValueFunc = [=]() -> Accuracy_t // Utilizes binary search if given square is between 0 and lookUpMap's size squared
			{
				constexpr T lookUpMap[] = {
					CAST(T, 0),    CAST(T, 1),    CAST(T, 4),    CAST(T, 9),      // 0, 1, 2, 3 
					CAST(T, 16),   CAST(T, 25),   CAST(T, 36),   CAST(T, 49),     // 4, 5, 6, 7
					CAST(T, 64),   CAST(T, 81),   CAST(T, 100),  CAST(T, 121),    // 8, 9, 10, 11
					CAST(T, 144),  CAST(T, 169),  CAST(T, 196),  CAST(T, 225),    // 12, 13, 14, 15
					CAST(T, 256),  CAST(T, 289),  CAST(T, 324),  CAST(T, 361),    // 16, 17, 18, 19
					CAST(T, 400),  CAST(T, 441),  CAST(T, 484),  CAST(T, 529),    // 20, 21, 22, 23
					CAST(T, 576),  CAST(T, 625),  CAST(T, 676),  CAST(T, 729),    // 24, 25, 26, 27
					CAST(T, 784),  CAST(T, 841),  CAST(T, 900),  CAST(T, 961),    // 28, 29, 30, 31
					CAST(T, 1024), CAST(T, 1089), CAST(T, 1156), CAST(T, 1225)    // 32, 33, 34, 35
				};
				constexpr auto lookUpMapSize = SizeOfCArray(lookUpMap);

				auto estimate = CAST(Accuracy_t, kmaths::secret::impl::BinarySearchClosestImpl(lookUpMap, square, 0, lookUpMapSize - 1, lookUpMapSize));
				if (estimate == 0)
				{
					estimate = square;
					do {
						estimate *= constants::ZeroPointFive<Accuracy_t>;
					} while (estimate * estimate > square);
				}
				return estimate;
			};

			Accuracy_t start = chooseStartValueFunc();

			if (start * start == square)
				return static_cast<T>(start);

			Accuracy_t result = start;
			Accuracy_t prevValue[2] = { constants::MinusOne<Accuracy_t>, constants::MinusOne<Accuracy_t> };

			const auto checkResultIsUnique = [&result, &prevValue]() {
				for (auto& prev : prevValue)
					if (prev == result)
						return false;
				return true;
			};

			while (checkResultIsUnique() && maxIterations-- > 0)
			{
				prevValue[Modulus(maxIterations, 2)] = result;

				// Bakhshali Method
				const auto a = (square - (result * result)) / (2 * result);
				const auto b = result + a;
				result = b - ((a * a) / (2 * b));

				// Heron's Method
				//result = CAST(T, zeroPointFive * (result + (square / result)));
			}

			return static_cast<T>(result);
#endif
		}

		template<typename T, typename ReturnType = T, class = std::enable_if_t<std::is_floating_point_v<T>>>
		USE_RESULT constexpr ReturnType RootImpl(T num, size_t root)
		{
			using namespace constants;
			constexpr auto one = constants::One<Accuracy_t>;
			constexpr auto minusOne = -one;
			constexpr auto zeroPointOne = constants::ZeroPointOne<Accuracy_t>;
			constexpr auto minusZeroPointOne = -zeroPointOne;

			const auto oneOverRoot = constants::OneOver<Accuracy_t>(root);

			if (IsNegative(num))
			{
				if ((root & 1) == 0) // Even root
				{
					throw klib::kDebug::NoRealRootError(num, root); // No real root
				}

				if (num == minusOne)
					return CAST(ReturnType, minusOne);
			}

			if (num == 0)
				return 0;

			if (num == one || root == one)
				return CAST(ReturnType, num);

			const auto chooseStartNumber = [&](auto number) -> Accuracy_t
			{
				constexpr auto ZeroPointFive = constants::ZeroPointFive<Accuracy_t>;

				auto maxIterations = 0;
				if constexpr(std::is_same_v<T, float>)
					maxIterations = 7;
				else
					maxIterations = 16;

				Accuracy_t estimate = 0;
				const bool isNegative = num < 0;

				if (isNegative)
					number = -num;

				if (num < 1 && num > -1)
				{
					Accuracy_t startVal = 0, endVal = 1;
					auto increment = number > 0 ? zeroPointOne : minusZeroPointOne;
					for (auto i = 0; i < maxIterations; ++i)
					{
						Accuracy_t compareVal = 0;
						do {
							endVal -= increment;
							estimate = (startVal + endVal) * ZeroPointFive;
							compareVal = PowerOfImpl<Accuracy_t>(estimate, root);
						} while (compareVal > number);

						startVal = estimate;
						endVal += increment;
						increment *= zeroPointOne;

						if (compareVal == number)
							break;
					}
				}
				else
				{
					estimate = number;
					do {
						estimate *= oneOverRoot;
					} while (PowerOfImpl<Accuracy_t>(estimate, root) > number);
				}

				return isNegative ? -estimate : estimate;
			};

			const Accuracy_t start = chooseStartNumber(num);

			if (PowerOfImpl(start, root) == num)
				return CAST(ReturnType, start);

			Accuracy_t result = start;
			Accuracy_t prev[2] = { minusOne, minusOne };

			constexpr auto checkResultIsUniqueFunc = [](auto result, auto (&prev)[2]) {
				for (auto& p : prev)
					if (p == result)
						return false;
				return true;
			};

			size_t iterations = 0;
			const auto size = SizeOfCArray(prev);
			while (checkResultIsUniqueFunc(result, prev))
			{
				prev[Modulus(iterations++, size)] = result;

				const auto x_power_of_root_minus_one = PowerOfImpl(result, root - 1);
				const auto num_over_x_pow_r_m_o = num / x_power_of_root_minus_one;
				const auto next_result = oneOverRoot * (num_over_x_pow_r_m_o - result);
				result += next_result;
			}

			return CAST(ReturnType, result);
		}

		// Approximation for the mathematical constant 'e^x' using the first n terms of the taylor series
		template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
		USE_RESULT constexpr T ExponentialImpl(T x) noexcept
		{
			using namespace kmaths::secret::impl;
			constexpr T e = CAST(T, constants::E);

			return PowerOfImpl<T>(e, x);
		}
	}

	template<typename T>
	USE_RESULT constexpr T LogGamma(T);

	// Continued Fractions Without Tears, Ian Richards 1981
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr Fraction RealToFraction(T x, const uint8_t dpAccuracy = 10) noexcept
	{
		using namespace kmaths::secret::impl;
		constexpr auto maxIterations = static_cast<BigInt_t>(1e6);
		BigInt_t iter = 0;

		const auto isNegative = x < 0;
		if (isNegative) x = -x;

		if (static_cast<Fraction::Numerator_Value_Type>(x) == x)
			return { static_cast<Fraction::Numerator_Value_Type>(x), 1, isNegative, false };

		const T error = PowerOfImpl(constants::ZeroPointOne<T>, Min(dpAccuracy, Max_Decimal_Precision_V<T>));

		const T x0 = x;
		size_t a(0);
		size_t b(1);
		size_t c(1);
		size_t d(0);
		size_t integer(0);

		do {
			integer = CAST(size_t, x);
			const Fraction::Numerator_Value_Type num = a + integer * c;
			const Fraction::Denominator_Value_Type den = b + integer * d;
			a = c;
			b = d;
			c = num;
			d = den;
			x = constants::OneOver<T>(x - CAST(T, integer));
			const auto diff = Abs<T>(x0 - (CAST(T, num) / den));
			if (error > diff) { return { num, den, isNegative, false }; }
		} while (iter++ < maxIterations);

		return { 0, 1, isNegative, false };
	}


	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T HandleFloatingPointError(T x) noexcept
	{
		if (x == 0)
			return 0;

		const auto x0 = x;

		constexpr auto epsilon = std::numeric_limits<T>::epsilon() * 10;

		const auto integer = Floor<T>(x);

		if (!IsDecimal<T>(x))
		{
			x = GetDecimals(x);
		}

		if (x != 0 && (epsilon >= x && x >= -epsilon))
			return (x0 - x);
		else if (!IsInteger(x0))
		{
			if (std::is_same_v<T, float>)
				return Round(x0, 5);
			else
				return Round(x0, 14);
		}
		else
			return x0;
	}

	template<typename T1, typename T2, typename T3>
	USE_RESULT constexpr T1 Clamp(const T1 value, const T2 min, const T3 max)
	{
		if (max <= min)
			throw klib::kDebug::MathsError("Max value cannot be less than or equal to min value");

		return Min(static_cast<T1>(max), Max(value, min));
	}

	template<typename T>
	USE_RESULT constexpr T AbsClamp(const T value, const T min, const T max) noexcept
	{
		return value >= 0 ? Clamp(value, min, max) : -Clamp(value, min, max);
	}

	template<typename T>
	USE_RESULT constexpr T GetRange(const T minVal, const T maxVal) noexcept
	{
		return maxVal - minVal;
	}

	template <typename T>
	USE_RESULT constexpr T LerpClampled(T a, T b, T t) noexcept
	{
		t = Clamp<T>(t, 0, 1);
		return a + t * (b - a);
	}

	template <typename T>
	USE_RESULT constexpr T LerpPartial(T a, T b, T t, T tMin, T tMax) noexcept
	{
		t = Clamp<T>(t, tMin, tMax);

		t -= tMin;
		t /= (tMax - tMin);

		return LerpClampled<T>(a, b, t);
	}

	template <typename T>
	USE_RESULT constexpr T GetTimeTakenForAcceleration(T initialVelocity, T distance, T finalVelocity) noexcept
	{
		const T timeResult = (finalVelocity - initialVelocity) / GetAccelerationOverTime(initialVelocity, distance);

		return timeResult;
	}

	template <typename T>
	USE_RESULT constexpr T LerpWithAcceleration(T initialVelocity, T currentTime, T distance) noexcept
	{
		const T acceleration = GetAccelerationOverTime<T>(initialVelocity, distance);
		const T result = (initialVelocity * currentTime) + ((acceleration / 2) * (currentTime * currentTime));
		const T currentVel = initialVelocity + (acceleration * currentTime);
		return result;
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Root(T num, size_t root)
	{
		using namespace kmaths::secret::impl;
		if constexpr(!std::is_floating_point_v<T>)
			return RootImpl<float, T>(CAST(float, num), root);
		else
			return RootImpl<T>(num, root);
	}

	// Bakhshali Method
	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Sqrt(T square)
	{
		using namespace kmaths::secret::impl;
		return static_cast<T>(SqrtImpl<ClosestFloat_t<T>>(static_cast<ClosestFloat_t<T>>(square)));
	}


	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T InvSqrt(T square)
	{
		using namespace kmaths::secret::impl;
		// Root<>(...) More accurate than the Sqrt<>(...)
		return constants::OneOver<ClosestFloat_t<T>>(Root<T>(square, 2));
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, T power) noexcept
	{
		using namespace kmaths::secret::impl;
		const auto fraction = RealToFraction<T>(power);

		const auto pow = fraction.isNegative
			? constants::OneOver<T>(PowerOfImpl<T>(base, fraction.numerator))
			: PowerOfImpl<T>(base, fraction.numerator);
		const auto powRoot = RootImpl<T>(pow, fraction.denominator);
		return powRoot;
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, size_t numerator, size_t denominator, bool isNegative = false) noexcept
	{
		using namespace kmaths::secret::impl;
		if (denominator == 0)
			return 0;

		const int8_t sign = isNegative ? -1 : 1;

		if constexpr(!std::is_floating_point_v<T>)
		{
			const auto pow = PowerOfImpl<float>(base, numerator * sign);
			const auto powRoot = RootImpl<float>(pow, denominator);
			return CAST(T, powRoot);
		}
		else
		{
			const auto pow = PowerOfImpl<T>(base, numerator * sign);
			const auto powRoot = RootImpl<T>(pow, denominator);
			return powRoot;
		}
	}

	// Natural Logarithm ////////////////////////////////////////////////////////////////////////////
	template<typename T>
	USE_RESULT constexpr T NaturalLogarithm(const T x)
	{
		using namespace kmaths::secret::impl;
		using constants::Accuracy_t;

		constexpr auto one = constants::One<T>;
		constexpr auto maxIter = static_cast<uint16_t>(2.048e3);

		if (0.01 < x && x < 100)
		{
			Accuracy_t log_result = 1.l;
			const Accuracy_t y = constants::Divide<Accuracy_t>(x - one, x + one);
			uint32_t denominator = 3;
			uint16_t iter = 2;

			do {
				log_result += PowerOfImpl<Accuracy_t>(y, iter) / denominator;
				denominator += 2;
			} while ((iter += 2) <= maxIter);

			const auto result = 2.l * y * log_result;
			return CAST(T, result);
		}

		return CAST(T, std::log(x));
	}

	template<typename T>
	USE_RESULT constexpr T Logarithm(T num, T base)
	{
		return NaturalLogarithm(num) / NaturalLogarithm(base);
	}

	template<typename T>
	USE_RESULT constexpr T Log10(const T x)
	{
		if constexpr(std::is_floating_point_v<T>)
		{
			constexpr auto ln10 = constants::Ln10<T>;
			return Round<T>(NaturalLogarithm<T>(x) / ln10, Max_Decimal_Precision_V<T>);
		}
		else
		{
			constexpr auto ln10 = constants::Ln10<float>;
			const auto result = Round<float>(NaturalLogarithm<float>(CAST(float, x)) / ln10, Max_Decimal_Precision_V<T>);
			return CAST(T, result);
		}
	}

	template<typename T>
	USE_RESULT constexpr T Log2(const T x)
	{
		if constexpr(std::is_floating_point_v<T>)
		{
			constexpr auto ln2 = constants::Ln2<T>;
			return Round<T>(NaturalLogarithm<T>(x) / ln2, Max_Decimal_Precision_V<T>);
		}
		else
		{
			constexpr auto ln2 = constants::Ln2<float>;
			const auto result = Round<float>(NaturalLogarithm<float>(CAST(float, x)) / ln2, Max_Decimal_Precision_V<T>);
			return CAST(T, result);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////



	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Exponential(T x) noexcept
	{
		using namespace kmaths::secret::impl;
		if constexpr(std::is_integral_v<T>)
			return CAST(T, ExponentialImpl<float>(CAST(float, x)));
		else
			return ExponentialImpl<T>(x);
	}

	template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
	USE_RESULT constexpr T Factorial_Integral(T n) noexcept
	{
		if (IsNegative(n)) return 0;

		if (n > 1)
			return (n * Factorial_Integral(n - 1));

		return 1;
	}


	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Gamma(T z)
	{
		using namespace constants;

		if (IsNegative(z)) return Zero<T>;

		constexpr auto gamma = constants::EulerGamma<Accuracy_t>;

		if (z < 0.001)
			return CAST(T, constants::OneOver<Accuracy_t>(z * (1.0l + gamma * z)));

		///////////////////////////////////////////////////////////////////////////
		// Second interval: [0.001, 12)

		if (z < 12.0)
		{
			// The algorithm directly approximates gamma over (1,2) and uses
			// reduction identities to reduce other arguments to this interval.

			Accuracy_t y = z;
			BigInt_t n = 0;
			const bool arg_was_less_than_one = (y < 1.0);

			// Add or subtract integers as necessary to bring y into (1,2)
			// Will correct for this below
			if (arg_was_less_than_one)
			{
				y += 1.0;
			}
			else
			{
				n = CAST(BigInt_t, (Floor(y))) - 1;  // will use n later
				y -= n;
			}

			// numerator coefficients for approximation over the interval (1,2)
			constexpr double p[] =
			{
				-1.71618513886549492533811E+0,
				 2.47656508055759199108314E+1,
				-3.79804256470945635097577E+2,
				 6.29331155312818442661052E+2,
				 8.66966202790413211295064E+2,
				-3.14512729688483675254357E+4,
				-3.61444134186911729807069E+4,
				 6.64561438202405440627855E+4
			};

			// denominator coefficients for approximation over the interval (1,2)
			constexpr double q[] =
			{
				-3.08402300119738975254353E+1,
				 3.15350626979604161529144E+2,
				-1.01515636749021914166146E+3,
				-3.10777167157231109440444E+3,
				 2.25381184209801510330112E+4,
				 4.75584627752788110767815E+3,
				-1.34659959864969306392456E+5,
				-1.15132259675553483497211E+5
			};

			Accuracy_t num = 0.0;
			Accuracy_t den = 1.0;
			int8_t i = 0;

			const Accuracy_t zn = y - 1;
			for (i = 0; i < 8; i++)
			{
				num = (num + p[i]) * zn;
				den = den * zn + q[i];
			}
			Accuracy_t result = num / den + 1.0;

			// Apply correction if argument was not initially in (1,2)
			if (arg_was_less_than_one)
			{
				// Use identity gamma(z) = gamma(z+1)/z
				// The variable "result" now holds gamma of the original y + 1
				// Thus we use y-1 to get back the orginal y.
				result /= (y - 1.0);
			}
			else
			{
				// Use the identity gamma(z+n) = z*(z+1)* ... *(z+n-1)*gamma(z)
				for (i = 0; i < n; i++)
					result *= y++;
			}

			return CAST(T, result);
		}

		///////////////////////////////////////////////////////////////////////////
		// Third interval: [12, infinity)

		if (z > 171.624)
		{
			// Correct answer too large to display. Force +infinity.
			constexpr T inf = constants::Infinity<T>;
			return inf;
		}

		return CAST(T, std::tgamma(z));
	}


	template<typename T>
	USE_RESULT constexpr T LogGamma(T z)
	{
		constexpr auto halfLogTwoPi = constants::Log_Tau_Over_2; // 0.91893853320467274178032973640562;

		if (z < 12.0)
		{
			return NaturalLogarithm(Abs(Gamma(z)));
		}

		// Abramowitz and Stegun 6.1.41
		// Asymptotic series should be good to at least 11 or 12 figures
		// For error analysis, see Whittiker and Watson
		// A Course in Modern Analysis (1927), page 252

		constexpr double c[] =
		{
			 1.0 / 12.0,
			-1.0 / 360.0,
			 1.0 / 1260.0,
			-1.0 / 1680.0,
			 1.0 / 1188.0,
			-691.0 / 360360.0,
			1.0 / 156.0,
			-3617.0 / 122400.0
		};

		const double zn = 1.0 / (z * z);
		double sum = c[7];
		for (int i = 6; i >= 0; i--)
		{
			sum *= zn;
			sum += c[i];
		}
		double series = sum / z;

		const double logGamma = (z - 0.5)
			* NaturalLogarithm(z) - z
			+ halfLogTwoPi + series;
		return CAST(T, logGamma);
	}

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Factorial_Floating_Point(T n) noexcept
	{
		return Gamma(n + 1);
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr T Factorial(T n)
	{
		if constexpr(std::is_floating_point_v<T>)
			return Factorial_Floating_Point<T>(n);
		else
			return Factorial_Integral<T>(n);
	}


}

