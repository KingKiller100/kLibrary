#pragma once
#include "kMathsFundamentals.hpp"
#include "Length_Type.hpp"

#include "../HelperMacros.hpp"

namespace kmaths
{
	template<typename T>
	USE_RESULT constexpr T Square(T x) noexcept
	{
		return x * x;
	}

	template<typename T>
	USE_RESULT constexpr T Cube(T x) noexcept
	{
		return x * x * x;
	}

	namespace secret::impl
	{
		template<typename T>
		USE_RESULT constexpr T PowerOfImpl(T base, BigInt_t power) noexcept
		{
#if MSVC_PLATFORM_TOOLSET > 142
			return CAST(T, std::pow<T, T>(base, power));
#else
			using namespace constants;
			
			if (IsNegative(power))
			{
				if _CONSTEXPR_IF(std::is_integral_v<T>)
					return Zero<T>();
				else
					base = OneOver<T, T>(base);
			}

			T result = One<T>();
			for (; power > 0; --power)
			{
				result *= base;
			}

			return result;
#endif // MSVC_PLATFORM_TOOLSET > 142
		}
	}

	template<typename T, class = std::enable_if_t<!std::is_floating_point_v<T>>>
	USE_RESULT constexpr T PowerOf(T base, BigInt_t power) noexcept
	{
		using namespace kmaths::secret::impl;
		const auto pow = PowerOfImpl<T>(base, power);
		return pow;
	}

	template<typename T>
	USE_RESULT constexpr T PowerOf10(T power) noexcept(std::is_arithmetic_v<T>)
	{
		using namespace kmaths::secret::impl;

		constexpr T _Small_powers_of_ten[] =
		{
		1,
		10,
		100,
		1'000,
		10'000,
		100'000,
		1'000'000,
		10'000'000,
		100'000'000,
		1'000'000'000
		};

		if (IsInteger(power)
			&& power < 10 && power >= 0)
			return _Small_powers_of_ten[power];

		constexpr auto ten = CAST(T, 10);
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			if (power < 0)
				return 0;
		}
		return PowerOfImpl<T>(ten, power);
	}

	template<typename T>
	USE_RESULT constexpr T PowerOf2(T power) noexcept(std::is_arithmetic_v<T>)
	{

	}
}
