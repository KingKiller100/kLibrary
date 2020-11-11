#include "pch.hpp"
#include "kLogDescriptor.hpp"

namespace klib::kLogs
{
	LogDescriptor::LogDescriptor(const LogLevel& lvl)
		: lvl(lvl)
		, info(lvl.ToString())
	{}

	LogDescriptor::LogDescriptor(const std::string_view& type)
		: lvl(LogLevel::BANR)
		, info(type)
	{}
}
