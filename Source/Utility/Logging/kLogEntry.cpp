#include "pch.hpp"
#include "kLogEntry.hpp"


namespace klib::kLogs
{
	LogEntry::LogEntry( const LogLevel lvl, LogProfile* profile, const LogMessage& message )
		: level( lvl )
		, profile( profile )
		, message( message )
	{}

	LogEntry::~LogEntry()
	= default;

	bool LogEntry::HasText( const std::string_view& msg ) const
	{
		return message.text.find( msg ) != std::string::npos;
	}

	const LogProfile& LogEntry::GetProfile() const
	{
		return *profile;
	}

	const LogMessage& LogEntry::GetMsg() const
	{
		return message;
	}
}
