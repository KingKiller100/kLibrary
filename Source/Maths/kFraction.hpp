#pragma once

#include "../HelperMacros.hpp"

#include <cstdint>
#include <type_traits>

namespace kmaths
{
	struct Fraction
	{
	public:
		using Sign_Value_Type = bool;
		using Numerator_Value_Type = size_t;
		using Denominator_Value_Type = size_t;

		constexpr Fraction() noexcept
			:numerator(0), denominator(1), isNegative(false), alwaysSimplify(true)
		{}
		
		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr Fraction(const T numerator = 0, const Denominator_Value_Type denominator = 1, const Sign_Value_Type isNegative = false, const bool simplified = true) noexcept
			: numerator(CAST(Numerator_Value_Type, numerator)), denominator(denominator), isNegative(isNegative), alwaysSimplify(simplified)
		{
			if (alwaysSimplify)
				Simplify();
		}

		~Fraction() noexcept
			= default;

		template<typename T>
		USE_RESULT constexpr T GetReal() const noexcept
		{
			if _CONSTEXPR_IF(std::is_signed_v<T>)
			{
				const T num = CAST(T, numerator);
				const T signedNum = isNegative ? -num : num;
				return  signedNum / CAST(T, denominator);
			}
			else
			{
				const T num = CAST(T, isNegative ? 0 : numerator);
				return  num / CAST(T, denominator);
			}
		}

		USE_RESULT constexpr int8_t GetSign() const noexcept
		{
			return isNegative ? -1 : 1;
		}

		constexpr void Simplify() noexcept
		{
			const auto lcf = LargestCommonFactor();
			const auto oneOverLCF = 1.f / lcf;
			numerator = CAST(Numerator_Value_Type, oneOverLCF * numerator);
			denominator = CAST(Numerator_Value_Type, oneOverLCF * denominator);

			if (numerator == denominator)
				numerator = denominator = 1;
		}

		USE_RESULT constexpr float LargestCommonFactor() const noexcept
		{
			float largestFactor = 1;
			const auto largest = numerator > denominator ? numerator : denominator;
			const auto loops = (largest / 2) + 1;

			for (Denominator_Value_Type i = 2; i < loops; ++i)
			{
				const double multiplier = 1.0 / i;
				const auto numFactor = CAST(float, numerator) * multiplier;
				const auto denFactor = CAST(float, denominator) * multiplier;

				if (size_t(numFactor) == numFactor
					&& size_t(denFactor) == denFactor)
				{
					largestFactor = float(i);
				}
			}

			return largestFactor;
		}


		// Operators

		// x Operators
		// With Fractions
		USE_RESULT constexpr Fraction operator+(const Fraction& other) const noexcept
		{
			const auto den = denominator * other.denominator;

			const auto num = numerator * other.denominator;
			const auto oNum = other.numerator * denominator;

			const auto sameSign = (!isNegative && !other.isNegative)
				|| (isNegative && other.isNegative);

			Fraction f;
			if (sameSign)
			{
				const auto n_Add_oN = num + oNum;
				f = Fraction(n_Add_oN, den, isNegative, alwaysSimplify);
			}
			else
			{
				const auto big = num > oNum ? num : oNum;
				const auto small = num < oNum ? num : oNum;
				const auto sign = big == num ? isNegative : other.isNegative;
				const auto n = big - small;
				f = Fraction(n, den, sign, alwaysSimplify);
			}

			return f;
		}

		USE_RESULT constexpr Fraction operator-(const Fraction& other) const noexcept
		{
			const auto f = Fraction(other.numerator, other.denominator, !other.isNegative, alwaysSimplify);
			return (*this + f);
		}

		USE_RESULT constexpr Fraction operator*(const Fraction& other) const noexcept
		{
			const auto num = numerator * other.numerator;
			const auto den = denominator * other.denominator;
			const auto sign = isNegative ^ other.isNegative;

			return Fraction(num, den, sign, alwaysSimplify);
		}

		USE_RESULT constexpr Fraction operator/(const Fraction& other) const noexcept
		{
			const auto f = Fraction(other.denominator, other.numerator, other.isNegative, alwaysSimplify);
			return (*this * f);
		}

		// With Real
		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		USE_RESULT constexpr Fraction operator+(const T real) const noexcept
		{
			const auto isRealNegative = real < 0;
			const auto f = Fraction(isRealNegative ? -real : real, 1, isRealNegative, alwaysSimplify);
			return (*this + f);
		}

		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		USE_RESULT constexpr Fraction operator-(const T real) const noexcept
		{
			const auto isRealNegative = real < 0;
			const auto f = Fraction(isRealNegative ? -real : real, 1, isRealNegative, alwaysSimplify);
			return (*this - f);
		}

		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		USE_RESULT constexpr Fraction operator*(const T real) const noexcept
		{
			const auto isRealNegative = real < 0;
			const auto num = numerator * (isRealNegative ? -real : real);
			const auto sign = isNegative ^ isRealNegative;

			return Fraction(num, denominator, sign, alwaysSimplify);
		}

		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		USE_RESULT constexpr Fraction operator/(const T real) const noexcept
		{
			const auto isRealNegative = real < 0;
			const auto den = denominator * (isRealNegative ? -real : real);
			const auto sign = isNegative ^ isRealNegative;

			return Fraction(numerator, den, sign, alwaysSimplify);
		}

		// x= Operators With Fractions
		constexpr Fraction& operator+=(const Fraction& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		constexpr Fraction& operator-=(const Fraction& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		constexpr Fraction& operator*=(const Fraction& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		constexpr Fraction& operator/=(const Fraction& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr Fraction& operator+=(const T real) noexcept
		{
			*this = *this + real;
			return *this;
		}

		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr Fraction& operator-=(const T real) noexcept
		{
			*this = *this - real;
			return *this;
		}

		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr Fraction& operator*=(const T real) noexcept
		{
			*this = *this * real;
			return *this;
		}

		template<typename T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr Fraction& operator/=(const T real) noexcept
		{
			*this = *this / real;
			return *this;
		}

	public:
		Numerator_Value_Type numerator;
		Denominator_Value_Type denominator;

		Sign_Value_Type isNegative;
		bool alwaysSimplify;
	};
}


