#pragma once

#include "kiLoggerDestination.hpp"

namespace klib::kLogs
{
	class DebugOutputWindowLogger final : public FormattedLogDestinationBase
	{
	public:
		DebugOutputWindowLogger();

		~DebugOutputWindowLogger();

		std::string_view GetName() const override;
		void AddEntry( const LogEntry& entry ) override;
		bool IsOpen() const override;
		void Open() override;
		void Close() override;
		
	private:
		std::string CreateLogText(const LogEntry& entry) const;

		void Flush(const std::string_view& msg) const;
		
	private:
		bool active;
	};
}
