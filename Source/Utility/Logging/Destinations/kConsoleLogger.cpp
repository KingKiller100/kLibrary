﻿#include <pch.hpp>
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

		ConsoleLogger::ConsoleLogger(std::string* pName)
			: active(false)
			, name(pName)
			, consoleColour(ConsoleColour::WHITE)
			, enableDebugStringOutput(false)
		{
			LogDestWithFormatSpecifier::SetFormat("&t", LogLevel::RAW);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&p]: &t"
				, LogLevel::TRC);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&p]: &t"
				, LogLevel::BNR);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&w]: &t"
				, LogLevel::DBG);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&w]: &t"
				, LogLevel::NRM);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&w]: &t"
				, LogLevel::INF);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&w]: &t"
				, LogLevel::WRN);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&w]: &t"
				"\n[Source]: &f [&l]"
				, LogLevel::ERR);

			LogDestWithFormatSpecifier::SetFormat(
				"[&hh:&zz:&ss:&ccc] [&n] [&w]: &t"
				"\n[File]: &f"
				"\n[Line]: &l"
				, LogLevel::FTL);
		}

		ConsoleLogger::~ConsoleLogger() noexcept
			= default;

		bool ConsoleLogger::Open()
		{
			active = true;
			return active;
		}

		bool ConsoleLogger::IsOpen() const
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
			// Message details
			const auto& t = msg.time;
			const auto& hour = t.GetHour();
			const auto& minute = t.GetMinute();
			const auto& second = t.GetSecond();
			const auto& milli = t.GetMillisecond();

			const auto& d = msg.date;
			const auto& day = d.GetDay();
			const auto& month = d.GetMonth();
			const auto& year = d.GetYear();

			const auto& text = msg.text;

			const auto& sourceInfo = msg.sourceInfo;

			// Description details
			const auto lvl = desc.lvl;
			const auto info = desc.info;

			const auto format = formatMap.at(lvl);

			std::string logLine = ToString(format,
				day,
				month,
				year,
				hour,
				minute,
				second,
				milli,
				*name,
				info,
				lvl.ToUnderlying(),
				text,
				sourceInfo.file,
				sourceInfo.line,
				sourceInfo.func
			);

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
			case LogLevel::ERR:
				consoleColour = ConsoleColour::SCARLET_RED;
				break;
			case LogLevel::FTL:
				consoleColour = ConsoleColour::RED_BG_WHITE_TEXT;
				break;
			case LogLevel::RAW:
			case LogLevel::BNR:
				consoleColour = ConsoleColour::WHITE;
				break;
			default:
				throw std::runtime_error("Unknown log level! Cannot map to a known console colour: "
					+ std::string(lvl.ToString()));
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
, *name
);

				Flush(padding);
				Flush(msg);
				Flush(padding);
				Flush("\n");
			}

			active = false;
		}

		void ConsoleLogger::SetDebugStringOutput(bool enable)
		{
			enableDebugStringOutput = enable;
		}

		void ConsoleLogger::Flush(const std::string_view& msg) const
		{
			std::scoped_lock scoped_lock(g_kConsoleLoggerMutex);

			if (!active)
				return;

			OutputToConsole(msg);
			OutputToDebugString(msg);
		}

		void ConsoleLogger::OutputToDebugString(const std::string_view& msg) const
		{
			if (enableDebugStringOutput)
			{
				kDebug::WriteToOutputWindow(msg);
			}
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
