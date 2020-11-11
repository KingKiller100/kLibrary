#include "pch.hpp"
#include "kCalendar.hpp"

#include "../String/kToString.hpp"
#include "../String/kStringManipulation.hpp"

#include <unordered_map>

namespace klib::kCalendar
{
	using namespace kString;

	static std::unordered_map<CalendarInfoSourceType, Date> dates = {
		 { CalendarInfoSourceType::LOCAL, Date(CalendarInfoSourceType::LOCAL) },
		 { CalendarInfoSourceType::SYSTEM, Date(CalendarInfoSourceType::SYSTEM) }
	};
	static std::unordered_map<CalendarInfoSourceType, Time> times = {
		 { CalendarInfoSourceType::LOCAL, Time(CalendarInfoSourceType::LOCAL) },
		 { CalendarInfoSourceType::SYSTEM, Time(CalendarInfoSourceType::SYSTEM) }
	};

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
		const auto time = ToString("%02d:%02d", hours, minutes);
		return time;
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds)  noexcept
	{
		if (hours > 23) hours = 23;
		if (minutes > 59) minutes = 59;
		if (seconds > 59) seconds = 59;
		const auto time = ToString("%02d:%02d:%02d", hours, minutes, seconds);
		return time;
	}

	std::string CreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) noexcept
	{
		if (milliseconds > 999) milliseconds = 999;
		auto time = CreateTime(hours, minutes, seconds);
		time.append(ToString(":%03d", milliseconds));
		return time;
	}

	std::string GetLocalStartTimeStr() noexcept
	{
		static const auto& info = times.at(CalendarInfoSourceType::LOCAL);
		static const auto str = info.ToString(Time::TimeComponent::MILLIS);
		return str;
	}

	std::string GetSystemStartTimeStr() noexcept
	{
		static const auto& info = times.at(CalendarInfoSourceType::SYSTEM);
		static const auto str = info.ToString(Time::TimeComponent::MILLIS);
		return str;
	}

	std::string GetLocalStartDateStr(const Date::DateNumericalSeparator separator) noexcept
	{
		static std::string local;
		if (local.empty())
		{
			static const auto& info = dates.at(CalendarInfoSourceType::LOCAL);
			local = info.ToString(separator);
			return local;
		}

		if (separator == Date::DateNumericalSeparator::SLASH)
		{
			if (local.find('/') == std::string::npos)
				local = kString::Replace<char>(local, '-', '/');
		}
		else
		{
			if (local.find('-') == std::string::npos)
				local = kString::Replace<char>(local, '/', '-');
		}

		return local;
	}

	std::string GetSystemStartDateStr(const Date::DateNumericalSeparator separator) noexcept
	{
		static std::string system;
		if (system.empty())
		{
			static const auto& info = dates.at(CalendarInfoSourceType::SYSTEM);
			system = info.ToString(separator);
			return system;
		}

		if (separator == Date::DateNumericalSeparator::SLASH)
		{
			if (system.find('/') == std::string::npos)
				system = kString::Replace<char>(system, '-', '/');
		}
		else
		{
			if (system.find('-') == std::string::npos)
				system = kString::Replace<char>(system, '/', '-');
		}

		return system;
	}
}


