#pragma once

#include "kiLoggerDestination.hpp"

#include "../../Misc/kConsoleColour.hpp"

#ifdef KLOG_OPT_DBG_STR
#	define KLOG_OPT_DBG_STR_ENABLED
#endif

namespace klib
{
	namespace kLogs
	{
		class LogDescriptor;
		class LogLevel;
		struct LogMessage;

		class ConsoleLogger final : public LogDestWithFormatSpecifier
		{
		public:
			ConsoleLogger(std::string* newName);
			~ConsoleLogger() noexcept;

			void SetName(std::string* newName) override;

			void AddEntry(const LogEntry& entry) override;

			bool Open() override;

			bool IsOpen() const override;

			void Close(const bool outputClosingMsg) override;

			void SetDebugStringOutput(bool enable);

		private:
			void UpdateConsoleColour(const LogLevel lvl);

			std::string CreateLogText(const LogMessage& msg, const LogDescriptor& desc) const;

			void Flush(const std::string_view& msg);
			void OutputToDebugString(const std::string_view& msg);
			void OutputToConsole(const std::string_view& msg) const;

		private:
			bool active;
			std::string* name;
			kMisc::ConsoleColour consoleColour;
			bool enableDebugStringOutput;
		};
	}
}
