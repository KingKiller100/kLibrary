#pragma once

#include "kiLoggerDestination.hpp"

#include "../../Misc/kConsoleColour.hpp"

namespace klib
{
	namespace kLogs
	{
		class LogLevel;
		struct LogMessage;
				
		class ConsoleLogger final : public iLoggerDestination
		{
		public:
			ConsoleLogger(const std::string& newName);
			~ConsoleLogger() noexcept;
			
			USE_RESULT std::string_view GetName() const override;
			void SetName(const std::string_view& newName) override;
			
			void OutputInitialized(const std::string_view& openingMsg) override;
			void AddEntry(const LogEntry& entry) override;
			
			bool Open() override;
			
			bool IsOpen() override;
			
			void Close(const bool outputClosingMsg) override;

		private:
			void UpdateConsoleColour(const LogLevel lvl);

			std::string CreateLogText(const LogMessage& msg, const LogDescriptor& desc) const;

			void Flush(const std::string_view& msg);
			void OutputToDebugString(const std::string_view& msg);
			void OutputToConsole(const std::string_view& msg) const;

		private:
			bool active;
			std::string name;
			kMisc::ConsoleColour consoleColour;
		};
	}
}
