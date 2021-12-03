#pragma once

#include "kiLoggerDestination.hpp"

namespace klib::kLogs
{
	class DebugOutputLogger final : public FormattedLogDestinationBase
	{
	public:
		DebugOutputLogger();

		~DebugOutputLogger();

		std::string_view GetName() const override;
		void AddEntry( const LogEntry& entry ) override;
		void AddRaw( const LogMessage& message ) override;
		bool IsOpen() const override;
		void Open() override;
		void Close() override;

	private:
		std::string CreateLogText( const LogEntry& entry ) const;
		std::string CreateRawLogText( const LogMessage& msg ) const;

		void Flush( const std::string_view& msg ) const;

	private:
		bool active;
	};
}
