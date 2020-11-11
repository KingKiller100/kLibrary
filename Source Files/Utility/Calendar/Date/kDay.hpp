#pragma once

#include "../Secret/kComponentToStringImpl.hpp"
#include "../../../HelperMacros.hpp"

#include <array>
#include <string>


namespace klib::kCalendar
{
	class Day : private CalendarComponentToStringImplExtended
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
		constexpr Day(std::uint16_t day, DayOfTheWeek dotw = MONDAY)
			: day(day), dayOfTheWeek(dotw)
		{}
		
		~Day() noexcept = default;

		USE_RESULT constexpr std::uint16_t GetValue() const noexcept
		{
			return day;
		}

		USE_RESULT constexpr DayOfTheWeek GetDayOfTheWeek() const noexcept
		{
			return dayOfTheWeek;
		}

		USE_RESULT static constexpr auto DayOfTheWeekToString(DayOfTheWeek day)
		{
			constexpr std::array<const char*,DaysInWeek> kCalendar_DaysOfTheWeek = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday"
			};

			return kCalendar_DaysOfTheWeek[day];
		}

		static std::string_view GetDaySuffix(const std::uint16_t day);
		
		USE_RESULT bool Verify() const;
		
		USE_RESULT std::string ToString(const std::string_view& format) const;

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return GetValue();
		}

		friend class Date;
		
	private:
		USE_RESULT std::string GetDayStr() const;
		USE_RESULT std::string GetDayOfTheWeekStr() const;
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
		
	private:
		const std::uint16_t day;
		const DayOfTheWeek dayOfTheWeek;
	};


	constexpr Day operator"" _d(unsigned long long day)
	{
		return Day(static_cast<std::uint16_t>(day));
	}
}

