﻿#pragma once
#include "kMathsConstants.hpp"
#include "kMathsFundamentals.hpp"

#include "../HelperMacros.hpp"

namespace kmaths
{
	template<class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	struct FloatBreakdown
	{
		size_t integers;
		size_t dpShifts;
		T decimals;
		bool isNeg;
	};

	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T GetDecimals(T x) noexcept
	{
		if (IsDecimal(x))
			return x;

		const auto isNeg = IsNegative(x);
		if (isNeg) x = Abs(x);
		x -= Floor(x);
		return isNeg ? -x : x;
	}

	template<typename T>
	USE_RESULT constexpr size_t GetDpShifts(T decimals)
	{
		size_t count = 0;
		if constexpr (std::is_floating_point_v<T>)
		{
			if (!constants::ApproximatelyZero<T>(decimals))
			{
				int maxIterations = 22;
				size_t magnitude = 1;
				T val = decimals;
				while (maxIterations-- > 0 && val < 1)
				{
					++count;
					magnitude *= 10;
					val = decimals * magnitude;
				}
			}
		}
		return count;
	}

	template<typename T>
	USE_RESULT constexpr FloatBreakdown<T> GetFigures(T val, size_t decimalPlaces)
	{
		const auto isNeg = IsNegative(val);
		val = Abs(val);
		const auto justIntegers = static_cast<size_t>(Floor(val));
		const auto justDecimals = GetDecimals(val);
		const auto dpShifts = GetDpShifts(justDecimals);
		return { justIntegers, dpShifts, justDecimals, isNeg };
	}
}