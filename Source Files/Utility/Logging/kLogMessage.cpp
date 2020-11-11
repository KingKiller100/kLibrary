#include <pch.hpp>
#include "kLogMessage.hpp"


namespace klib::kLogs
{
		LogMessage::LogMessage(const char* text, const char* file,
		                   const std::int32_t line, CalendarInfoSourceType calendarInfoSource)
			: time (calendarInfoSource)
			, date(calendarInfoSource)
			, text(text)
			, file(file)
			, line(line)
		{}

		LogMessage::LogMessage(const std::string& text, const std::string_view& file, const std::int32_t line,
			CalendarInfoSourceType calendarInfoSource)
			: time(calendarInfoSource)
			, date(calendarInfoSource)
			, text(text)
			, file(file)
			, line(line)
		{}

		LogMessage::LogMessage(const std::string& text, const LogMessage& other)
			: time(other.time)
			, date(other.date)
			, text(text)
			, file(other.file)
			, line(other.line)
		{}
}
