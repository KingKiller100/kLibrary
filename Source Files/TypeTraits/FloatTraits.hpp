#pragma once

#include "../HelperMacros.hpp"

#include <type_traits>
#include <limits>

namespace klib::type_trait
{
	union SinglePointPrecision
	{
		float f;

		struct
		{
			unsigned mantissa : 23;
			unsigned exponent : 8;
			unsigned sign : 1;
		} parts;
		std::uint32_t u;
	};

	union DoublePointPrecision
	{
		double f;

		struct
		{
			unsigned long long mantissa : 52;
			unsigned long long exponent : 11;
			unsigned long long sign : 1;
		} parts;

		std::uint64_t u;
	};

	static constexpr std::array<std::uint32_t, std::numeric_limits<float>::digits> g_SingleMantissaMask = {
		1u << 23, 1u << 22, 1u << 21,
		1u << 20, 1u << 19, 1u << 18, 1u << 17,
		1u << 16, 1u << 15, 1u << 14, 1u << 13,
		1u << 12, 1u << 11, 1u << 10, 1u << 9,
		1u << 8, 1u << 7, 1u << 6, 1u << 5,
		1u << 4, 1u << 3, 1u << 2, 1u << 1,
		1u << 0
	};

	static constexpr std::array<std::uint64_t, std::numeric_limits<double>::digits> g_DoubleMantissaMask = {
		1ull << 52, 1ull << 51, 1ull << 50, 1ull << 49,
		1ull << 48, 1ull << 47, 1ull << 46, 1ull << 45,
		1ull << 44, 1ull << 43, 1ull << 42, 1ull << 41,
		1ull << 40, 1ull << 39, 1ull << 38, 1ull << 37,
		1ull << 36, 1ull << 35, 1ull << 34, 1ull << 33,
		1ull << 32, 1ull << 31, 1ull << 30, 1ull << 29,
		1ull << 28, 1ull << 27, 1ull << 26, 1ull << 25,
		1ull << 24, 1ull << 23, 1ull << 22, 1ull << 21,
		1ull << 20, 1ull << 19, 1ull << 18, 1ull << 17,
		1ull << 16, 1ull << 15, 1ull << 14, 1ull << 13,
		1ull << 12, 1ull << 11, 1ull << 10, 1ull << 9,
		1ull << 8, 1ull << 7, 1ull << 6, 1ull << 5,
		1ull << 4, 1ull << 3, 1ull << 2, 1ull << 1,
		1ull << 0
	};

	template<class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr auto GetMask()
	{
		if _CONSTEXPR_IF(std::is_same_v<T, float>)
			return g_SingleMantissaMask;
		else
			return g_DoubleMantissaMask;
	}

	template<class T>
	struct FloatTraits
	{
		static_assert(std::is_floating_point_v<T>, "Type entered is not recognized as a floating point type");

		using Unsigned_t = std::conditional_t<sizeof(T) == 4, std::uint32_t, std::uint64_t>;
		using Precision_t = std::conditional_t<std::is_same_v<T, float>, SinglePointPrecision, DoublePointPrecision>;
		using Limits_t = std::numeric_limits<T>;


		static constexpr auto& Mask = GetMask<T>();
		static constexpr auto Bytes = sizeof(T);
		static constexpr auto Mantissa = Limits_t::digits;
		static constexpr auto Exponent = Bytes - Mantissa;
		static constexpr auto DotIndex = Mantissa - 3;

		USE_RESULT static constexpr Unsigned_t BitCast(T val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p.u;
		}

		USE_RESULT static constexpr Unsigned_t AsUint(T val) noexcept
		{
			return static_cast<Unsigned_t>(val);
		}

		USE_RESULT static constexpr Precision_t Parts(T val) noexcept
		{
			Precision_t p;
			p.f = val;
			return p;
		}
	};

}
