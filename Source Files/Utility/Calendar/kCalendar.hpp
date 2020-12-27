#pragma once

#include "Date/kDate.hpp"

#include "Time/kTime.hpp"

#include <string>

namespace klib
{
	namespace kCalendar
	{
		template<typename Date, typename Time>
		class BasicCalendar
		{
		public:
			using Date_t = Date;
			using Time_t = Time;

		public:
			constexpr BasicCalendar() noexcept
				= default;

			constexpr BasicCalendar(const CalendarInfoSourceType sourceType = CalendarInfoSourceType::LOCAL) noexcept
				: d(sourceType)
				, t(sourceType)
			{}

			constexpr BasicCalendar(const Date& newDate, const Time& newTime) noexcept
				: d(newDate)
				, t(newTime)
			{}

			BasicCalendar(const BasicCalendar&) noexcept = default;
			BasicCalendar(BasicCalendar&&) noexcept = default;

			constexpr BasicCalendar& operator=(const BasicCalendar&) noexcept = default;
			constexpr BasicCalendar& operator=(BasicCalendar&&) noexcept = default;

			constexpr const Date& GetDate() const noexcept
			{
				return d;
			}

			constexpr Date& GetDate() noexcept
			{
				return d;
			}

			constexpr const Time& GetTime() const noexcept
			{
				return t;
			}

			constexpr Time& GetTime() noexcept
			{
				return t;
			}

		private:
			Date d;
			Time t;
		};

		using GregorianCalendar = BasicCalendar<Date, Time>;

		std::string GetDateInTextFormat(const Date::DateTextLength format, const CalendarInfoSourceType source = CalendarInfoSourceType::LOCAL) noexcept;
		std::string GetDateInNumericalFormat(const Date::DateNumericalSeparator separator, const CalendarInfoSourceType  source = CalendarInfoSourceType::LOCAL) noexcept;

		std::uint16_t GetComponentOfTime(const Time::TimeComponent timeComponent, const CalendarInfoSourceType source = CalendarInfoSourceType::LOCAL);
		std::string GetTimeText(CalendarInfoSourceType calendarInfo = CalendarInfoSourceType::LOCAL) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds) noexcept;
		std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kCalendar;
#endif
}
