#pragma once

#include "kLogLevel.hpp"
#include <string>

namespace klib
{
	namespace kLogs
	{
		class LogDescriptor
		{
		public:
			explicit LogDescriptor(const LogLevel& lvl);
			explicit LogDescriptor(const std::string_view& type);

			const LogLevel lvl;
			const std::string info;
		};
	}
}
