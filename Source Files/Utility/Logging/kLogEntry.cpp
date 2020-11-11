#include "pch.hpp"
#include "kLogEntry.hpp"


namespace klib::kLogs
{
	LogEntry::LogEntry(const LogMessage& message, const LogDescriptor& descriptor)
		: message(message)
		, descriptor(descriptor)
	{}

	LogEntry::~LogEntry()
		= default;

	bool LogEntry::HasText(const std::string_view& msg) const
	{
		return message.text.find(msg) != std::string::npos;
	}

	bool LogEntry::HasDescription(const LogDescriptor& desc) const
	{
		return  (descriptor.lvl == desc.lvl) | (descriptor.info == desc.info);
	}

	bool LogEntry::HasDescription(const std::string_view& desc) const
	{
		return HasDescription(LogDescriptor(desc));
	}

	bool LogEntry::HasDescription(const LogLevel desc) const
	{
		return HasDescription(LogDescriptor(desc));
	}

	const LogMessage& LogEntry::GetMsg() const
	{
		return message;
	}

	const LogDescriptor& LogEntry::GetDescriptor() const
	{
		return descriptor;
	}
}
