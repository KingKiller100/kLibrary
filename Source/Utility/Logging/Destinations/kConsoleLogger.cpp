#include <pch.hpp>
#include "kConsoleLogger.hpp"

#include "../kLogLevel.hpp"
#include "../kLogEntry.hpp"

#include "../../Calendar/kCalendar.hpp"
#include "../../Calendar/kCalendarToString.hpp"
#include "../../String/kToString.hpp"

#include <mutex>
#include <Windows.h>


namespace klib
{
	using namespace kString;
	using namespace kCalendar;
	using namespace kMisc;

	namespace kLogs
	{
		namespace
		{
			std::mutex g_kConsoleLoggerMutex;
		}

		ConsoleLogger::ConsoleLogger()
			: active( false )
			, consoleColours( ConsoleColour::WHITE )
		{
			FormattedLogDestinationBase::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n]: &t"
			);

			FormattedLogDestinationBase::SetRawFormat(
				"[&hh:&zz:&ss:&ccc]: &t"
			);

			consoleColours.emplace( LogLevel::TRC, ConsoleColour::LIGHT_GREEN );
			consoleColours.emplace( LogLevel::DBG, ConsoleColour::AQUA_BLUE );
			consoleColours.emplace( LogLevel::INF, ConsoleColour::LIGHT_GREY );
			consoleColours.emplace( LogLevel::WRN, ConsoleColour::YELLOW );
			consoleColours.emplace( LogLevel::ERR, ConsoleColour::SCARLET_RED );
			consoleColours.emplace( LogLevel::FTL, ConsoleColour::RED_BG_WHITE_TEXT );
		}

		ConsoleLogger::~ConsoleLogger() noexcept
		= default;

		std::string_view ConsoleLogger::GetName() const
		{
			return "Console Logger";
		}

		void ConsoleLogger::Open()
		{
			active = true;
		}

		bool ConsoleLogger::IsOpen() const
		{
			return active;
		}

		void ConsoleLogger::AddEntry( const LogEntry& entry )
		{
			const auto logLine = CreateLogText( entry );


			Flush( logLine, UpdateConsoleColour( entry.GetProfile().GetLevel() ) );
		}

		std::string ConsoleLogger::CreateLogText( const LogEntry& entry ) const
		{
			const auto& msg = entry.GetMsg();
			const auto& profile = entry.GetProfile();

			// Level
			const auto lvl = profile.GetLevel();

			// Profile
			const auto& name = profile.GetName();

			// Message details
			const auto& time = msg.time;
			const auto& hour = time.GetHour();
			const auto& minute = time.GetMinute();
			const auto& second = time.GetSecond();
			const auto& milli = time.GetMillisecond();

			const auto& date = msg.date;
			const auto& day = date.GetDay();
			const auto& month = date.GetMonth();
			const auto& year = date.GetYear();

			const auto& text = msg.text;

			// Profile name

			std::string logLine = ToString( messageFormat,
				day,
				month,
				year,
				hour,
				minute,
				second,
				milli,
				name,
				lvl.ToString(),
				lvl.ToUnderlying(),
				text
			);

			logLine.push_back( '\n' );

			return logLine;
		}

		void ConsoleLogger::AddRaw( const LogMessage& message )
		{
			Flush( CreateRawLogText( message ), ConsoleColour::WHITE );
		}

		std::string ConsoleLogger::CreateRawLogText( const LogMessage& msg ) const
		{
			// Message details
			const auto& time = msg.time;
			const auto& hour = time.GetHour();
			const auto& minute = time.GetMinute();
			const auto& second = time.GetSecond();
			const auto& milli = time.GetMillisecond();

			const auto& date = msg.date;
			const auto& day = date.GetDay();
			const auto& month = date.GetMonth();
			const auto& year = date.GetYear();

			const auto& text = msg.text;

			// Profile name

			std::string logLine = ToString( rawMessageFormat,
				day,
				month,
				year,
				hour,
				minute,
				second,
				milli,
				text
			);

			logLine.push_back( '\n' );

			return logLine;
		}

		void ConsoleLogger::Flush( const std::string_view& msg, kMisc::ConsoleColour textColour ) const
		{
			std::scoped_lock scoped_lock( g_kConsoleLoggerMutex );

			if ( !active )
				return;

			SetConsoleColour( textColour );
			std::printf( "%s", msg.data() );
			SetConsoleColour( ConsoleColour::WHITE );
		}

		void ConsoleLogger::SetConsoleColour( kMisc::ConsoleColour colour ) const
		{
			const WORD attributes = colour.ToUnderlying();
			auto* handle = ::GetStdHandle( STD_OUTPUT_HANDLE );

			SetConsoleTextAttribute( handle, attributes );
		}

		kMisc::ConsoleColour ConsoleLogger::UpdateConsoleColour( const LogLevel lvl )
		{
			return consoleColours.at( lvl );
		}

		void ConsoleLogger::Close()
		{
			active = false;
		}

		void ConsoleLogger::LinkColour( LogLevel lvl, kMisc::ConsoleColour colour )
		{
			consoleColours.at( lvl ) = colour;
		}
	}
}
