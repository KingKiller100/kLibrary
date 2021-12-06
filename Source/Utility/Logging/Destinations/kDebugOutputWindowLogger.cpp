#include "pch.hpp"

#include "kDebugOutputWindowLogger.hpp"
#include "../kLogEntry.hpp"

#include "../../Calendar/kCalendar.hpp"
#include "../../Calendar/kCalendarToString.hpp"
#include "../../Debug/kDebugger.hpp"

#include <mutex>


namespace klib::kLogs
{
	namespace
	{
		std::mutex g_kDebugOutputLoggerMutex;
	}

	DebugOutputLogger::DebugOutputLogger()
		: active( false )
	{
		FormattedLogDestinationBase::SetFormat(
			"[&hh:&zz:&ss:&ccc] [&n] [&p]: &t"
		);

		FormattedLogDestinationBase::SetRawFormat(
			"[&hh:&zz:&ss:&ccc]: &t"
		);
	}

	DebugOutputLogger::~DebugOutputLogger()
	= default;

	std::string_view DebugOutputLogger::GetName() const
	{
		return "Debug Console Logger";
	}

	void DebugOutputLogger::AddEntry( const LogEntry& entry )
	{
		const auto& msg = entry.GetMsg();
		const auto& profile = entry.GetProfile();
		const auto logLine = CreateLogText( entry );

		Flush( logLine );
	}

	std::string DebugOutputLogger::CreateLogText( const LogEntry& entry ) const
	{
		const auto& msg = entry.GetMsg();
		const auto& profile = entry.GetProfile();

		// Level
		const auto lvl = profile.GetLevel();

		// Profile
		const auto& name = profile.GetName();

		// Message details
		const auto& time = msg.time;
		const auto& hour = time.GetHour();
		const auto& minute = time.GetMinute();
		const auto& second = time.GetSecond();
		const auto& milli = time.GetMillisecond();

		const auto& date = msg.date;
		const auto& day = date.GetDay();
		const auto& month = date.GetMonth();
		const auto& year = date.GetYear();

		const auto& text = msg.text;

		// Profile name

		std::string logLine = kString::ToString( messageFormat,
			day,
			month,
			year,
			hour,
			minute,
			second,
			milli,
			name,
			lvl.ToString(),
			lvl.ToUnderlying(),
			text
		);

		logLine.push_back( '\n' );

		return logLine;
	}

	void DebugOutputLogger::AddRaw( const LogMessage& message )
	{
		Flush( CreateRawLogText( message ) );
	}

	std::string DebugOutputLogger::CreateRawLogText( const LogMessage& msg ) const
	{
		// Message details
		const auto& time = msg.time;
		const auto& hour = time.GetHour();
		const auto& minute = time.GetMinute();
		const auto& second = time.GetSecond();
		const auto& milli = time.GetMillisecond();

		const auto& date = msg.date;
		const auto& day = date.GetDay();
		const auto& month = date.GetMonth();
		const auto& year = date.GetYear();

		const auto& text = msg.text;

		// Profile name

		std::string logLine = kString::ToString( rawMessageFormat,
			day,
			month,
			year,
			hour,
			minute,
			second,
			milli,
			text
		);

		logLine.push_back( '\n' );

		return logLine;
	}


	bool DebugOutputLogger::IsOpen() const
	{
		return active;
	}

	void DebugOutputLogger::Open()
	{
		active = true;
	}

	void DebugOutputLogger::Close()
	{
		active = false;
	}

	void DebugOutputLogger::Flush( const std::string_view& msg ) const
	{
		std::scoped_lock lock( g_kDebugOutputLoggerMutex );

		kDebug::WriteToOutputWindow( msg );
	}
}
