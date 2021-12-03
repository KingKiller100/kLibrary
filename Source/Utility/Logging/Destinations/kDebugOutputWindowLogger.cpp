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
		std::mutex g_kConsoleLoggerMutex;
	}

	DebugOutputWindowLogger::DebugOutputWindowLogger()
		: active( false )
	{
		FormattedLogDestinationBase::SetFormat(
			"[&hh:&zz:&ss:&ccc] [&n] [&p]: &t"
		);
	}

	DebugOutputWindowLogger::~DebugOutputWindowLogger()
	= default;

	std::string_view DebugOutputWindowLogger::GetName() const
	{
		return "Debug Console Logger";
	}

	void DebugOutputWindowLogger::AddEntry( const LogEntry& entry )
	{
		const auto& msg = entry.GetMsg();
		const auto& profile = entry.GetProfile();
		const auto logLine = CreateLogText( entry );

		Flush( logLine );
	}

	std::string DebugOutputWindowLogger::CreateLogText( const LogEntry& entry ) const
	{
		const auto& msg = entry.GetMsg();
		const auto& profile = entry.GetProfile();

		// Level
		const auto lvl = entry.GetLevel();

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

		std::string logLine = kString::ToString(messageFormat,
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

		logLine.push_back('\n');

		return logLine;
	}

	bool DebugOutputWindowLogger::IsOpen() const
	{
		return active;
	}

	void DebugOutputWindowLogger::Open()
	{
		active = true;
	}

	void DebugOutputWindowLogger::Close()
	{
		active = false;
	}

	void DebugOutputWindowLogger::Flush( const std::string_view& msg ) const
	{
		kDebug::WriteToOutputWindow( msg );
	}
}
