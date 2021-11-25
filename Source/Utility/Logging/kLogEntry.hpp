﻿#pragma once

#include "kLogProfile.hpp"
#include "kLogMessage.hpp"
#include "kLogLevel.hpp"

namespace klib::kLogs
{
	class LogEntry
	{
	public:
		LogEntry( const LogLevel lvl, const LogProfile& profile, const LogMessage& data );
		~LogEntry();

		USE_RESULT bool HasText( const std::string_view& msg ) const;

		USE_RESULT const LogLevel& GetLevel() const;
		USE_RESULT const LogProfile& GetProfile() const;
		USE_RESULT const LogMessage& GetMsg() const;

	private:
		LogLevel level;
		LogProfile profile;
		LogMessage message;
	};
}
