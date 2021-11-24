#pragma once

#include "kLogProfile.hpp"
#include "kLogMessage.hpp"

namespace klib::kLogs
{
	class LogEntry
	{
	public:
		LogEntry(const LogProfile& profile, const LogMessage& data);
		~LogEntry();

		USE_RESULT bool HasText(const std::string_view& msg) const;

		USE_RESULT const LogProfile& GetProfile() const;
		USE_RESULT const LogMessage& GetMsg() const;
		
	private:
		LogProfile profile;
		LogMessage message;
	};
}

