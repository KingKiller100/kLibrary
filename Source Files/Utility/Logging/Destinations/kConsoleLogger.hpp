#pragma once

#include "kiLoggerDestination.hpp"

#include "../../EnumHelper/EnumHelper.hpp"

namespace klib
{
	namespace kLogs
	{
		class LogLevel;
		struct LogMessage;
		
		ENUM_CLASS(ConsoleColour, unsigned short,
		           NAVY_BLUE = 1,
		           DARK_GREEN,
		           AQUA_BLUE,
		           DARK_RED,
		           PURPLE,
		           MUSTARD_YELLOW,
		           LIGHT_GREY,
		           GREY,
		           DARK_BLUE,
		           LIGHT_GREEN,
		           LIGHT_BLUE,
		           SCARLET_RED,
		           VIOLET_PURPLE,
		           YELLOW,
		           WHITE,
		           RED_BG_WHITE_TEXT = 79
		);
		
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
			ConsoleColour currentConsoleColour;
		};
	}
}