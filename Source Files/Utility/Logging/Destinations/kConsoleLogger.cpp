#include <pch.hpp>
#include "kConsoleLogger.hpp"

#include "../kLogLevel.hpp"
#include "../kLogEntry.hpp"

#include "../../Calendar/kCalendar.hpp"
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
		
		ConsoleLogger::ConsoleLogger(const std::string& newName)
			: active(false)
			, name(newName)
			, consoleColour(ConsoleColour::WHITE)
		{}

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

		std::string_view ConsoleLogger::GetName() const
		{
			return name;
		}

		void ConsoleLogger::SetName(const std::string_view& newName)
		{
			name = newName;
		}

		void ConsoleLogger::OutputInitialized(const std::string_view& openingMsg)
		{
			if (!IsOpen())
				return;

			constexpr char newLine[] = "\n";
			const std::string msg(openingMsg);
			const std::string padding(73, '*');
			const std::string prefixSpaces(19, ' ');
			const auto date = GetDateInTextFormat(Date::DateTextLength::SHORT);
			const auto time = GetTimeText();
			
			std::string format = newLine + padding + newLine;
			format += prefixSpaces + "{0} - ";
			format += openingMsg;
			format += newLine + prefixSpaces + date + "    " + time;
			format += newLine + padding + newLine;
			const auto opener = ToString(format, name);

			Flush(opener);
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

			if (desc.lvl == LogLevel::VBAT)
			{
				logLine = msg.text;
			}
			else
			{
				const auto timeStr = msg.time.ToString();
				const auto dateStr = msg.date.ToString(Date::SLASH);

				logLine = ToString("[{0}] [{1}] [{2}] [{3}]:  {4}",
					dateStr,
					timeStr,
					name,
					desc.lvl.ToUnderlying(),
					msg.text);
			}

			if (desc.lvl >= LogLevel::ERRR)
			{
				logLine.append(ToString(R"(
               [FILE]: {0}
               [LINE]: {1})",
					msg.file,
					msg.line)
				);
			}

			logLine.push_back('\n');

			return logLine;
		}

		void ConsoleLogger::UpdateConsoleColour(const LogLevel lvl)
		{
			switch (lvl.ToEnum())
			{
			case LogLevel::DBUG:
				consoleColour = ConsoleColour::AQUA_BLUE;
				break;
			case LogLevel::NORM:
				consoleColour = ConsoleColour::LIGHT_GREY;
				break;
			case LogLevel::INFO:
				consoleColour = ConsoleColour::LIGHT_GREEN;
				break;
			case LogLevel::WARN:
				consoleColour = ConsoleColour::YELLOW;
				break;
			case LogLevel::VBAT:
			case LogLevel::BANR:
				consoleColour = ConsoleColour::WHITE;
				break;
			case LogLevel::ERRR:
				consoleColour = ConsoleColour::SCARLET_RED;
				break;
			case LogLevel::FATL:
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
