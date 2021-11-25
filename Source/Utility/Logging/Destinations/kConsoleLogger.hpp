#pragma once

#include "kiLoggerDestination.hpp"

#include "../../Misc/kConsoleColour.hpp"

namespace klib
{
	namespace kLogs
	{
		ENUM_CLASS_FWD_DCL( LogLevel );

		class ConsoleLogger final : public LogDestWithFormatSpecifier
		{
		public:
			ConsoleLogger();
			~ConsoleLogger() noexcept;

			std::string_view GetName() const override;

			void AddEntry( const LogEntry& entry ) override;

			void Open() override;

			bool IsOpen() const override;

			void Close() override;

		private:
			void UpdateConsoleColour( const LogLevel lvl );

			std::string CreateLogText( const LogEntry& entry ) const;

			void Flush( const std::string_view& msg ) const;
			void OutputToConsole( const std::string_view& msg ) const;
			
		private:
			bool active;
			kMisc::ConsoleColour consoleColour;
		};
	}
}
