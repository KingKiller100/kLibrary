#pragma once
#include "../HelperMacros.hpp"
#include "kMathsFundamentals.hpp"

namespace kmaths
{
	namespace secret::impl
	{
		constexpr size_t GetCountImpl() noexcept
		{
			return 0;
		}
		template<typename T, typename ...Ts>
		constexpr size_t GetCountImpl(const T&, const Ts& ...args) noexcept
		{
			return 1 + GetCountImpl(args...);
		}
	}

	template<typename T, typename ...Ts>
	constexpr size_t Count(const T& arg, const Ts& ...args) noexcept
	{
		using namespace secret::impl;
		const size_t count(GetCountImpl(arg, args...));
		return count;
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr size_t CountIntegerDigits(T x) noexcept
	{
		constexpr T ten = CAST(T, 10);
		size_t count = 1;
		x = Abs(x);
		while (ten <= x)
		{
			x /= ten;
			count++;
		}
		return count;
	}
}
