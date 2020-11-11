#pragma once

#include "../HelperMacros.hpp"

#include <limits>
#include <type_traits>

namespace kmaths
{
	using Length_Type = unsigned short;

	using Big_Int_Type = std::make_signed_t<size_t>;

	template<typename T>
	struct _Max_Decimal_Precision
	{
		static constexpr size_t value = 0;
	};

	template<>
	struct _Max_Decimal_Precision<double>
	{
		static constexpr size_t value = 14;
	};

	template<>
	struct _Max_Decimal_Precision<long double>
	{
		static constexpr size_t value = 14;
	};

	template<>
	struct _Max_Decimal_Precision<float>
	{
		static constexpr size_t value = 5;
	};

	template<typename T>
	struct Max_Decimal_Precision : _Max_Decimal_Precision<ONLY_TYPE(T)>
	{};

	template<typename T>
	constexpr auto Max_Decimal_Precision_V = Max_Decimal_Precision<T>::value;
}