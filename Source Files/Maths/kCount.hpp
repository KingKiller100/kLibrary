#pragma once
#include "../HelperMacros.hpp"
#include "kMathsFundamentals.hpp"

namespace kmaths
{
	namespace secret::impl
	{
		constexpr size_t GetCountImpl()
		{
			return 0;
		}
		template<typename T, typename ...Ts>
		constexpr size_t GetCountImpl(const T&, const Ts& ...args)
		{
			return 1 + GetCountImpl(args...);
		}
	}

	template<typename T, typename ...Ts>
	constexpr size_t Count(const T& arg, const Ts& ...args)
	{
		using namespace secret::impl;
		const auto count(GetCountImpl(arg, args...));
		return count;
	}

	template<typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr size_t CountIntegerDigits(T x) noexcept
	{
		constexpr T ten = CAST(T, 10);
		size_t count = 1;
		while (ten <= Abs(x))
		{
			x /= ten;
			count++;
		}
		return count;
	}
}
