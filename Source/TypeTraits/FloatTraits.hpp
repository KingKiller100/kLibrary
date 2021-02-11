#pragma once

#include "../HelperMacros.hpp"

#include "TraitsBase.hpp"

#include <type_traits>
#include <limits>
#include <cstdint>

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
	
	template<>
	struct Traits<float>
	{
	public:
		using Type = float;
		using Unsigned_t = std::uint32_t;
		using Precision_t = SingleFloatPrecision;
		using Limits_t = std::numeric_limits<Type>;

		static constexpr auto Bytes = sizeof(Type);
		static constexpr auto Bits = Bytes * 8;
		static constexpr auto Mantissa = Limits_t::digits;
		static constexpr auto Exponent = Bytes - Mantissa;
		static constexpr auto DotIndex = Mantissa - 3;

		USE_RESULT static constexpr Unsigned_t UintBitCast(Type val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.u;
		}

		USE_RESULT static constexpr typename Precision_t::Parts Parts(Type val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.parts;
		}
	};
	
	template<>
	struct Traits<double>
	{
	public:
		using Type = double;
		using Unsigned_t = std::uint64_t;
		using Precision_t = DoubleFloatPrecision;
		using Limits_t = std::numeric_limits<Type>;

		static constexpr auto Bytes = sizeof(Type);
		static constexpr auto Bits = Bytes * 8;
		static constexpr auto Mantissa = Limits_t::digits;
		static constexpr auto Exponent = Bytes - Mantissa;
		static constexpr auto DotIndex = Mantissa - 3;

		USE_RESULT static constexpr Unsigned_t UintBitCast(Type val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.u;
		}

		USE_RESULT static constexpr typename Precision_t::Parts Parts(Type val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.parts;
		}
	};
	
	template<>
	struct Traits<long double>
	{
	public:
		using Type = long double;
		using Unsigned_t = std::uint64_t;
		using Precision_t = DoubleFloatPrecision;
		using Limits_t = std::numeric_limits<Type>;

		static constexpr auto Bytes = sizeof(Type);
		static constexpr auto Bits = Bytes * 8;
		static constexpr auto Mantissa = Limits_t::digits;
		static constexpr auto Exponent = Bytes - Mantissa;
		static constexpr auto DotIndex = Mantissa - 3;

		USE_RESULT static constexpr Unsigned_t UintBitCast(Type val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.u;
		}

		USE_RESULT static constexpr typename Precision_t::Parts Parts(Type val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.parts;
		}
	};

}
