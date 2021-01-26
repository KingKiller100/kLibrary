#pragma once
#include "kPowerOf.hpp"
#include "kMathsFundamentals.hpp"
#include "Length_Type.hpp"

#include "../HelperMacros.hpp"

namespace kmaths
{
	template<typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Round(T value, const uint8_t decimalPoints = Max_Decimal_Precision_V<T>) noexcept
	{
		using namespace kmaths::secret::impl;
		using namespace constants;

		const auto isNegative = IsNegative(value);
		if (isNegative)
			value = -value;

		const auto accuracy = PowerOfImpl<Accuracy_t>(CAST(Accuracy_t, 10), decimalPoints);
		const auto accuracyInverse = constants::OneOver<Accuracy_t>(accuracy);
		const auto dpShifts = constants::ZeroPointFive<> * accuracyInverse;

		const auto valuePlusDpsByAcc = (CAST(constants::Accuracy_t, value) + dpShifts) * accuracy;
		const auto sigFigs = Floor(valuePlusDpsByAcc); // significant figures
		const T roundedValue = CAST(T, sigFigs * accuracyInverse);
		return isNegative ? -roundedValue : roundedValue;
	}
}
