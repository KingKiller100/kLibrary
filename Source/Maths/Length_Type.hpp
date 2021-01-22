#pragma once

#include "../HelperMacros.hpp"

#include <cstdint>
#include <type_traits>

namespace kmaths
{
	using Length_t = std::uint16_t;

	using BigInt_t = std::make_signed_t<size_t>;

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
	struct Max_Decimal_Precision : _Max_Decimal_Precision<klib::type_trait::Simplify_t<T>>
	{};

	template<typename T>
	constexpr auto Max_Decimal_Precision_V = Max_Decimal_Precision<T>::value;
}