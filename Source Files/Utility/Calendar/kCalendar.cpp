#include "pch.hpp"
#include "kCalendar.hpp"

#include "../String/kToString.hpp"
#include "../String/kStringTricks.hpp"

namespace klib::kCalendar
{
	using namespace kString;

	std::uint16_t GetComponentOfTime(const Time::TimeComponent timeComponent, const CalendarInfoSourceType  source)
	{
		const auto now = Time(source);
		return now.GetComponent(timeComponent);
	}

	std::string GetTimeText(CalendarInfoSourceType calendarInfo)  noexcept
	{
		const auto now = Time(calendarInfo);
		return now.ToString(Time::TimeComponent::MILLIS);
	}

	std::string GetDateInTextFormat(const Date::DateTextLength format, const CalendarInfoSourceType  source) noexcept
	{
		const auto date = Date(source);
		return date.ToString(format);
	}

	std::string GetDateInNumericalFormat(const Date::DateNumericalSeparator separator, const CalendarInfoSourceType  source) noexcept
	{
		const auto date = Date(source);
		return date.ToString(separator);
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes) noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		const auto time = ToString("{0:2}:{1:2}", hours, minutes);
		return time;
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds)  noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		if (seconds > 59) seconds = 59;
		const auto time = ToString("{0:2}:{1:2}:{2:2}", hours, minutes, seconds);
		return time;
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept
	{
		if (milliseconds > 999) milliseconds = 999;
		auto time = CreateTime(hours, minutes, seconds);
		time.append(ToString(":{0:3}", milliseconds));
		return time;
	}
}


