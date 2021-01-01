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
			if (power == 0)
				return constants::One<T>();
			if (power == 1)
				return base;
			if (power == 2)
				return Square(base);
			if (power == 3)
				return Cube(base);

			const T temp = PowerOfImpl(base, power >> 1);

			if (power % 2 == 0)
				return Square(temp);
			else if (IsNegative(power))
				return Square(temp) / base;
			else
				return base * Square(temp);
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
	USE_RESULT constexpr T PowerOf2(BigInt_t power) noexcept(std::is_arithmetic_v<T>)
	{
		if (IsNegative(power))
			return static_cast<T>(1.l / PowerOf2<T>(Abs(power)));
		else
			return static_cast<T>(static_cast<size_t>(1) << power);
	}
}
