#pragma once

#include "../HelperMacros.hpp"

#include <type_traits>
#include <limits>

namespace klib::type_trait
{
	template<class T>
	struct FloatToUint
	{
		static_assert(std::is_floating_point_v<T>, "Type entered is not recognized as a floating point type");

		using Unsigned_t = std::conditional_t<sizeof(T) == 4, std::uint32_t, std::uint64_t>;

		static constexpr auto Bytes = sizeof(T);
		static constexpr auto Mantissa = std::numeric_limits<T>::digits;
		static constexpr auto Exponent = Bytes - Mantissa;
		static constexpr auto DotIndex = Mantissa - 3;

		static USE_RESULT constexpr Unsigned_t BitCast(T val) noexcept
		{
			return *(Unsigned_t*)std::addressof(val);
		}
	};

	template<class T>
	struct UintToFloat
	{
		static_assert(std::is_unsigned_v<T>, "Type entered is not recognized as an unsigned type");

		using Floating_t = std::conditional_t<sizeof(T) == 4, float, double>;

		static USE_RESULT constexpr Floating_t BitCast(T val) noexcept
		{
			return *(Floating_t*)std::addressof(val);
		}
	};

}
