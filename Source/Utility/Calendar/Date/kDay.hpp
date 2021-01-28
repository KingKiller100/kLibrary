#pragma once

#include "../../../HelperMacros.hpp"
#include "../Secret/kComponentToStringImpl.hpp"

#include <array>
#include <string>

#include "../../../Template/kSimpleOperators.hpp"


namespace klib::kCalendar
{
	class Day : private CalendarComponentToStringImplExtended, public kTemplate::SimpleComparisonOperators<Day>
	{
	public:
		enum DayOfTheWeek : unsigned char
		{
			SUNDAY = 0, MONDAY, TUESDAY,
			WEDNESDAY, THURSDAY, FRIDAY,
			SATURDAY,
		};

		static constexpr auto FormatToken = 'd';

		static constexpr size_t TotalDaysInMonths[] =
		{ 0, 31, 59, 90, 120, 151,
			181, 212, 243, 273, 304, 334, 365 };

		static constexpr size_t TotalDaysInMonthsLeap[] =
		{ 0, 31, 60, 91, 121, 152,
			182, 213, 244, 274, 305, 335, 366 };

		static constexpr size_t DaysInWeek = 7;
		static constexpr size_t DaysInYear = 365;
		static constexpr size_t DaysInLeapYear = 366;

	public:
		constexpr Day(std::uint16_t day = 1, DayOfTheWeek dotw = MONDAY) noexcept
			: day(day), dayOfTheWeek(dotw)
		{}

		~Day() noexcept = default;

		USE_RESULT constexpr std::uint16_t GetValue() const noexcept
		{
			return day;
		}

		USE_RESULT constexpr void SetValue(std::uint16_t d)
		{
			day = d;
		}

		USE_RESULT constexpr DayOfTheWeek GetDayOfTheWeek() const noexcept
		{
			return dayOfTheWeek;
		}

		USE_RESULT constexpr void SetValue(DayOfTheWeek dotw)
		{
			dayOfTheWeek = dotw;
		}

		USE_RESULT static constexpr auto DayOfTheWeekToString(DayOfTheWeek day)
		{
			constexpr std::array<const char*, DaysInWeek> kCalendar_DaysOfTheWeek = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday"
			};

			return kCalendar_DaysOfTheWeek[day];
		}

		USE_RESULT bool Verify() const;

		USE_RESULT std::string ToString(const std::string_view& format) const;

		template<typename Target_t, class = std::enable_if_t<
			std::is_arithmetic_v<Target_t>
			>>
			constexpr operator Target_t() const
		{
			return static_cast<Target_t>(GetValue());
		}

		friend class Date;

	private:
		USE_RESULT std::string GetDayStr() const;
		USE_RESULT std::string GetDayOfTheWeekStr() const;
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;

	private:
		std::uint16_t day;
		DayOfTheWeek dayOfTheWeek;
	};


	constexpr Day operator"" _d(unsigned long long day)
	{
		return Day(static_cast<std::uint16_t>(day));
	}
}

