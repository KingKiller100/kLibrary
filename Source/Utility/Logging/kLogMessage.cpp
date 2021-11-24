#include <pch.hpp>
#include "kLogMessage.hpp"


namespace klib::kLogs
{
	using namespace kCalendar;

	LogMessage::LogMessage( const char* text, const char* file, const std::int32_t line, CalendarInfoSourceType calendarInfoSource )
		: time( calendarInfoSource )
		, date( calendarInfoSource )
		, text( text )
		, sourceInfo( file, line, "" )
	{}

	LogMessage::LogMessage( const std::string_view& text, const std::string_view& file, const std::int32_t line, CalendarInfoSourceType calendarInfoSource )
		: time( calendarInfoSource )
		, date( calendarInfoSource )
		, text( text )
		, sourceInfo( file.data(), line, "" )
	{}

	LogMessage::LogMessage( const char* text, const kDebug::SourceInfo& sourceInfo, CalendarInfoSourceType calendarInfoSource )
		: time( calendarInfoSource )
		, date( calendarInfoSource )
		, text( text )
		, sourceInfo( sourceInfo )
	{}

	LogMessage::LogMessage( const std::string_view& text, const kDebug::SourceInfo& sourceInfo, CalendarInfoSourceType calendarInfoSource )
		: time( calendarInfoSource )
		, date( calendarInfoSource )
		, text( text )
		, sourceInfo( sourceInfo )
	{}

	LogMessage::LogMessage( const std::string& text, const LogMessage& other )
		: time( other.time )
		, date( other.date )
		, text( text )
		, sourceInfo( other.sourceInfo )
	{}
}
