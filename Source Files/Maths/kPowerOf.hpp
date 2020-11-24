#pragma once
#include "kMathsFundamentals.hpp"
#include "Length_Type.hpp"

#include "../HelperMacros.hpp"

namespace kmaths
{
	namespace secret::impl
	{
		template<typename T>
		USE_RESULT constexpr T PowerOfImpl(T base, BigInt_t power) noexcept
		{
#if MSVC_PLATFORM_TOOLSET > 142
			return CAST(T, pow(base, power));
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
				return temp * temp;
			else if (IsNegative(power))
				return (temp * temp) / base;
			else
				return base * temp * temp;
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
		constexpr auto ten = CAST(T, 10);
		if _CONSTEXPR_IF(!std::is_floating_point_v<T>)
		{
			if (power < 0)
				return 0;
		}
		return PowerOfImpl<T>(ten, power);
	}
}
