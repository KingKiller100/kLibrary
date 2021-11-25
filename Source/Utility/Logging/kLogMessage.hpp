#pragma once

#include "../Calendar/Time/kTime.hpp"
#include "../Calendar/Date/kDate.hpp"
#include <string>

namespace klib::kLogs
{
	struct LogMessage
	{
		LogMessage(
			const char* text
			, kCalendar::CalendarInfoSourceType calendarInfoSource = kCalendar::CalendarInfoSourceType::LOCAL
		);
		LogMessage(
			const std::string_view& text
			, kCalendar::CalendarInfoSourceType calendarInfoSource = kCalendar::CalendarInfoSourceType::LOCAL
		);
		LogMessage( const std::string& text, const LogMessage& other );

		const kCalendar::Time time;
		const kCalendar::Date date;
		const std::string text;
	};
}
