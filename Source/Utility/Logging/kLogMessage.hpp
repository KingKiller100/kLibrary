#pragma once

#include "../Calendar/Time/kTime.hpp"
#include "../Calendar/Date/kDate.hpp"
#include <string>

namespace klib::kLogs
{
	struct LogMessage
	{
		explicit LogMessage(
			const char* text
			, kCalendar::CalendarInfoSourceType calendarInfoSource = kCalendar::CalendarInfoSourceType::LOCAL
		);
		explicit LogMessage(
			const std::string_view& text
			, kCalendar::CalendarInfoSourceType calendarInfoSource = kCalendar::CalendarInfoSourceType::LOCAL
		);
		explicit LogMessage( const std::string& text, const LogMessage& other );

		const kCalendar::Time time;
		const kCalendar::Date date;
		const std::string text;
	};
}
