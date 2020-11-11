#pragma once

#include "kLogMessage.hpp"
#include "kLogDescriptor.hpp"

namespace klib::kLogs
{
	class LogEntry
	{
	public:
		LogEntry(const LogMessage& data, const LogDescriptor& descriptor);
		~LogEntry();

		USE_RESULT bool HasText(const std::string_view& msg) const;
		USE_RESULT bool HasDescription(const LogDescriptor& desc) const;
		USE_RESULT bool HasDescription(const std::string_view& desc) const;
		USE_RESULT bool HasDescription(const LogLevel desc) const;

		USE_RESULT const LogMessage& GetMsg() const;
		USE_RESULT const LogDescriptor& GetDescriptor() const;
		
	private:
		LogMessage message;
		LogDescriptor descriptor;
	};
}

