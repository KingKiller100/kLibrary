#include <pch.hpp>
#include "kLogMessage.hpp"


namespace klib::kLogs
{
	using namespace kCalendar;

	LogMessage::LogMessage( const char* text, CalendarInfoSourceType calendarInfoSource )
		: time( calendarInfoSource )
		, date( calendarInfoSource )
		, text( text )
	{}

	LogMessage::LogMessage( const std::string_view& text, CalendarInfoSourceType calendarInfoSource )
		: time( calendarInfoSource )
		, date( calendarInfoSource )
		, text( text )
	{}
}
