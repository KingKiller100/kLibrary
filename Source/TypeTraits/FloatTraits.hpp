#pragma once

#include "../HelperMacros.hpp"

#include <type_traits>
#include <limits>

namespace klib::type_trait
{
	union SingleFloatPrecision
	{
		float f;
		std::uint32_t u;

		struct Parts
		{
			unsigned mantissa : FLT_MANT_DIG - 1;
			unsigned exponent : (CHAR_BIT * sizeof(float) - FLT_MANT_DIG);
			unsigned sign : 1;
		} parts;
	};

	union DoubleFloatPrecision
	{
		double f;
		std::uint64_t u;

		struct Parts
		{
			unsigned long long mantissa : (DBL_MANT_DIG - 1);
			unsigned long long exponent : (CHAR_BIT * sizeof(double) - DBL_MANT_DIG);
			unsigned long long sign : 1;
		} parts;
	};

	template<class T
	, typename UnsignedT = std::conditional_t<sizeof(T) == 4, std::uint32_t, std::uint64_t>
	, typename PrecisionT = std::conditional_t<std::is_same_v<T, float>, SingleFloatPrecision, DoubleFloatPrecision>
	>
	struct FloatTraits
	{
	public:
		static_assert(std::is_floating_point_v<T>, "Type entered is not recognized as a floating point type");

		using Unsigned_t = UnsignedT;
		using Precision_t = PrecisionT;
		using Limits_t = std::numeric_limits<T>;

		static constexpr auto Bytes = sizeof(T);
		static constexpr auto Mantissa = Limits_t::digits;
		static constexpr auto Exponent = Bytes - Mantissa;
		static constexpr auto DotIndex = Mantissa - 3;

		USE_RESULT static constexpr Unsigned_t UintBitCast(T val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.u;
		}

		USE_RESULT static constexpr typename Precision_t::Parts Parts(T val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.parts;
		}
	};

}
