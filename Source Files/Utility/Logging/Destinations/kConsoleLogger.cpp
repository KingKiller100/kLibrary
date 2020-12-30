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
		std::mutex g_kConsoleLoggerMutex;

		ConsoleLogger::ConsoleLogger(std::string* newName)
			: active(false)
			, name(newName)
			, consoleColour(ConsoleColour::WHITE)
		{
			LogDestWithFormatSpecifier::SetFormat("[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&n] [&l]: &t");
		}

		ConsoleLogger::~ConsoleLogger() noexcept
			= default;

		bool ConsoleLogger::Open()
		{
			active = true;
			return active;
		}

		bool ConsoleLogger::IsOpen()
		{
			return active;
		}

		void ConsoleLogger::SetName(std::string* newName)
		{
			name = newName;
		}

		void ConsoleLogger::AddEntry(const LogEntry& entry)
		{
			if (!IsOpen())
				return;

			const auto& msg = entry.GetMsg();
			const auto& desc = entry.GetDescriptor();
			const auto logLine = CreateLogText(msg, desc);

			UpdateConsoleColour(desc.lvl);
			Flush(logLine);
		}

		std::string ConsoleLogger::CreateLogText(const LogMessage& msg, const LogDescriptor& desc) const
		{
			std::string logLine;

			if (desc.lvl == LogLevel::RAW)
			{
				logLine = msg.text;
			}
			else
			{
				const auto& t = msg.time;
				const auto& hour = t.GetHour();
				const auto& minute = t.GetMinute();
				const auto& second = t.GetSecond();
				const auto& milli = t.GetMillisecond();

				const auto& d = msg.date;
				const auto& day = d.GetDay();
				const auto& month = d.GetMonth();
				const auto& year = d.GetYear();

				logLine = ToString(LogDestWithFormatSpecifier::logFormat,
					day,
					month,
					year,
					hour,
					minute,
					second,
					milli,
					*name,
					desc.lvl.ToUnderlying(),
					msg.text);
			}

			if (desc.lvl >= LogLevel::ERR)
			{
				logLine.append(ToString(R"(
               [FILE]: {0}
               [LINE]: {1})",
					msg.sourceInfo.file
					, msg.sourceInfo.line)
				);

				if (!msg.sourceInfo.func.empty())
				{
					logLine.append(ToString(R"(
               [FUNC]: {0})",
						msg.sourceInfo.func));
				}
			}

			logLine.push_back('\n');

			return logLine;
		}

		void ConsoleLogger::UpdateConsoleColour(const LogLevel lvl)
		{
			switch (lvl.ToEnum())
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
			case LogLevel::RAW:
			case LogLevel::BNR:
				consoleColour = ConsoleColour::WHITE;
				break;
			case LogLevel::ERR:
				consoleColour = ConsoleColour::SCARLET_RED;
				break;
			case LogLevel::FTL:
				consoleColour = ConsoleColour::RED_BG_WHITE_TEXT;
				break;
			default:
				throw std::runtime_error("Unknown log level! Cannot map to a known console colour: " + std::string(lvl.ToString()));
			}
		}



		void ConsoleLogger::Close(const bool outputClosingMsg)
		{
			if (outputClosingMsg)
			{
				const std::string padding(73, '*');
				const auto msg
					= ToString(R"(
               {0}: Console Logger Concluded
)"
, name
);

				Flush(padding);
				Flush(msg);
				Flush(padding);
			}

			active = false;
		}

		void ConsoleLogger::Flush(const std::string_view& msg)
		{
			std::scoped_lock<decltype(g_kConsoleLoggerMutex)> scoped_lock(g_kConsoleLoggerMutex);

			if (!active)
				return;

			OutputToConsole(msg);
			OutputToDebugString(msg);
		}

		void ConsoleLogger::OutputToDebugString(const std::string_view& msg)
		{
#if defined(KLIB_DEBUG) || defined(_DEBUG)
			OutputDebugStringA(msg.data());
#endif
		}

		void ConsoleLogger::OutputToConsole(const std::string_view& msg) const
		{
			static constexpr auto whiteText = CAST(WORD, ConsoleColour::WHITE);

			auto* handle = GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleTextAttribute(handle, consoleColour);
			std::printf("%s", msg.data());

			if (whiteText != consoleColour)
				SetConsoleTextAttribute(handle, whiteText);
		}
	}
}
