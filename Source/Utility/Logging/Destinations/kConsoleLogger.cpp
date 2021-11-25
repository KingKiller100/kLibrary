#include <pch.hpp>
#include "kConsoleLogger.hpp"

#include "../kLogLevel.hpp"
#include "../kLogEntry.hpp"

#include "../../Calendar/kCalendar.hpp"
#include "../../Calendar/kCalendarToString.hpp"
#include "../../Debug/kDebugger.hpp"
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
			, consoleColour( ConsoleColour::WHITE )
		{
			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n]: &t"
			);
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

			UpdateConsoleColour( entry.GetLevel() );
			Flush( logLine );
		}

		std::string ConsoleLogger::CreateLogText( const LogEntry& entry ) const
		{
			const auto& msg = entry.GetMsg();
			const auto& profile = entry.GetProfile();

			// Level
			const auto lvl = entry.GetLevel();

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

		void ConsoleLogger::UpdateConsoleColour( const LogLevel lvl )
		{
			switch ( lvl.ToEnum() )
			{
			case LogLevel::DBG:
				consoleColour = ConsoleColour::AQUA_BLUE;
				break;
			case LogLevel::NRM:
				consoleColour = ConsoleColour::LIGHT_GREY;
				break;
			case LogLevel::INF:
				consoleColour = ConsoleColour::LIGHT_GREEN;
				break;
			case LogLevel::WRN:
				consoleColour = ConsoleColour::YELLOW;
				break;
			case LogLevel::ERR:
				consoleColour = ConsoleColour::SCARLET_RED;
				break;
			case LogLevel::FTL:
				consoleColour = ConsoleColour::RED_BG_WHITE_TEXT;
				break;
			default:
				throw std::runtime_error( "Unknown log level! Cannot map to a known console colour: "
					+ std::string( lvl.ToString() ) );
			}
		}

		void ConsoleLogger::Close()
		{
			active = false;
		}

		void ConsoleLogger::Flush( const std::string_view& msg ) const
		{
			std::scoped_lock scoped_lock( g_kConsoleLoggerMutex );

			if ( !active )
				return;

			OutputToConsole( msg );
		}

		void ConsoleLogger::OutputToConsole( const std::string_view& msg ) const
		{
			static constexpr ConsoleColour whiteText = static_cast<WORD>( ConsoleColour::WHITE );

			auto* handle = ::GetStdHandle( STD_OUTPUT_HANDLE );

			SetConsoleTextAttribute( handle, consoleColour.ToUnderlying() );
			std::printf( "%s", msg.data() );

			if ( whiteText != consoleColour )
				SetConsoleTextAttribute( handle, whiteText.ToUnderlying() );
		}
	}
}
