#pragma once

#include "../Calendar/Time/kTime.hpp"
#include "../Calendar/Date/kDate.hpp"
#include "../Debug/Source/kSourceInfo.hpp"
#include <string>
#include <cstdint>

namespace klib::kLogs
{
	using namespace kCalendar;

	struct LogMessage
	{
		LogMessage(const char* text, const char* file = __FILE__,
			const std::int32_t line = __LINE__, CalendarInfoSourceType calendarInfoSource = CalendarInfoSourceType::LOCAL);
		LogMessage(const std::string& text, const std::string_view& file = __FILE__,
			const std::int32_t line = __LINE__, CalendarInfoSourceType calendarInfoSource = CalendarInfoSourceType::LOCAL);
		LogMessage(const char* text, const kDebug::SourceInfo& sourceInfo
			, CalendarInfoSourceType calendarInfoSource = CalendarInfoSourceType::LOCAL);
		LogMessage(const std::string_view& text, const kDebug::SourceInfo& sourceInfo
			, CalendarInfoSourceType calendarInfoSource = CalendarInfoSourceType::LOCAL);
		LogMessage(const std::string& text, const LogMessage& other);
		
		const Time time;
		const Date date;
		const std::string text;
		const kDebug::SourceInfo sourceInfo;
	};
}
