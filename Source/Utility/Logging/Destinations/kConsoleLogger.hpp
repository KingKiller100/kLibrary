#pragma once

#include "kiLoggerDestination.hpp"
#include "../kLogLevel.hpp"

#include "../../Misc/kConsoleColour.hpp"

namespace klib
{
	namespace kLogs
	{
		ENUM_CLASS_FWD_DCL( LogLevel );

		class ConsoleLogger final : public FormattedLogDestinationBase
		{
		public:
			ConsoleLogger();
			~ConsoleLogger() noexcept;

			std::string_view GetName() const override;

			void AddEntry( const LogEntry& entry ) override;

			void AddRaw(const LogMessage& message) override;

			void Open() override;

			bool IsOpen() const override;

			void Close() override;

			void LinkColour(LogLevel lvl, kMisc::ConsoleColour colour);

		private:
			kMisc::ConsoleColour UpdateConsoleColour( const LogLevel lvl );

			[[nodiscard]] std::string CreateLogText( const LogEntry& entry ) const;
			[[nodiscard]] std::string CreateRawLogText( const LogMessage& msg ) const;

			void Flush( const std::string_view& msg, kMisc::ConsoleColour textColour ) const;

			void SetConsoleColour(kMisc::ConsoleColour colour) const;
			
		private:
			bool active;
			std::unordered_map<LogLevel::Value, kMisc::ConsoleColour> consoleColours;
		};
	}
}
