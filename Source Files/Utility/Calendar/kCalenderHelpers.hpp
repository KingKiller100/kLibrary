#pragma once
#include <chrono>

#include "Date/kDay.hpp"
#include "Date/kMonth.hpp"
#include "Date/kYear.hpp"

namespace klib::kCalendar
{
	// Date
	USE_RESULT constexpr double YearsFromDays(const size_t days)
	{
		const auto noOfLeaps = days / Year::LeapYearFrequency / Day::DaysInYear;
		const auto years = days / Day::DaysInYear;
		const auto remainingDays = days % Day::DaysInYear;
		const auto year = years
			- static_cast<double>(noOfLeaps) / Day::DaysInYear
			+ static_cast<double>(remainingDays) / Day::DaysInYear;
		return year;
	}

	USE_RESULT constexpr double YearsFromMonths(const size_t months)
	{
		const auto year = static_cast<double>(months) / Month::MonthsInYear;
		return year;
	}

	USE_RESULT constexpr double MonthsFromDays(const size_t days)
	{
		constexpr auto divider = 30.4167;
		const auto months = days / divider;
		return months;
	}

	// Time
	template<typename DestTimeT, typename SourceTimeT, typename = std::enable_if_t<
		!std::is_arithmetic_v<DestTimeT>
		>>
		USE_RESULT constexpr DestTimeT TimeConverter(const TimeComponentBase<SourceTimeT>& source)
	{
		using namespace std::chrono;
		using DestUnderlyingT = typename DestTimeT::UnderlyingT;

		const auto val =
			duration_cast<DestUnderlyingT>(source.GetUnderlying());
		const DestTimeT dest = DestTimeT(val.count());
		return dest;
	}
}
