#pragma once

#include "../kLogLevel.hpp"

#include "../../String/Format/FormatSymbols.hpp"
#include "../../String/Format/Stringify/kStringifyInteger.hpp"

#include <string>
#include <unordered_map>

namespace klib::kLogs
{
	class LogEntry;

	class iLoggerDestination
	{
	public:
		virtual ~iLoggerDestination() = default;

		/**
		 * \brief
		 *		Change name of the logger
		 * \param[in] newName
		 *		STL string representing a name
		 */
		virtual void SetName(std::string* newName) = 0;

		/**
		 * \brief
		 *		Formats log message and level to the appropriate log message and then caches it
		 * \param[in] entry
		 *		Log entry containing message, time, date, log level, file and file line
		 */
		virtual void AddEntry(const LogEntry& entry) = 0;

		/**
		 * \brief
		 *		Returns if destination is open
		 * \return
		 *		TRUE if open, FALSE if not unopened
		 */
		virtual bool IsOpen() const = 0;

		/**
		 * \brief
		 *		Opens up the log destination
		 */
		virtual bool Open() = 0;

		/**
		 * \brief
		 *		Closes the log destination
		 *	\param[in] outputClosingMsg
		 *			whether we should output the close message when closing
		 */
		virtual void Close(const bool outputClosingMsg) = 0;
	};

	struct LogDestWithFormatSpecifier : public iLoggerDestination
	{
	public:
		/**
		 * \brief
		 *		Sets the format of all log message
		 *		[Example] "[&dd/&mm/&yyyy] [&hh:&zz:&ss] [&n]: &t" can result in
		 *		an output like this "[01/01/1970] [01:12:59] [Logger]: Pass Test!"
		 * \param format
		 *		Format of the log message for the destination logger
		 *		Declare each detail specifier item with a '&' character
		 *		Using multiple calls of the same specifiers gives different results
		 *		Detail specifiers:
		 *		- d/D = Day
		 *		- m/M = Month
		 *		- y/Y = Year
		 *		- h/H = Hours
		 *		- z/Z = Minutes
		 *		- s/S = Seconds
		 *		- c/C = Milliseconds
		 *		- n/N = Name
		 *		- p/P = Log descriptor [text]
		 *		- w/W = Log descriptor [numeric value]
		 *		- t/T = Log message
		 *		- f/F = source file
		 *		- l/L = source line
		 *		- e/E = source function
		 * \param lvl
		 *		Log level to change the format for
		 */
		virtual void SetFormat(const std::string_view& format, const LogLevel lvl) noexcept
		{
			auto& logFormat = formatMap[lvl];

			logFormat.clear();

			const auto realFormat = kString::ToLower(format);

			for (size_t i = 0; i < realFormat.size(); ++i)
			{
				const auto& letter = realFormat[i];
				if (letter != DetailSpecifier)
					logFormat.push_back(format[i]);
				else
				{
					logFormat.push_back('{');

					const auto identifier = realFormat[i + 1];

					const auto& fi = LogFormatSpecifiersMap.at(identifier);

					logFormat.append(fi);

					const auto firstIndex = i + 1;

					auto lastIndex = realFormat.find_first_not_of(identifier, firstIndex);
					if (lastIndex == std::string::npos)
						lastIndex = firstIndex + 1;

					const auto count = lastIndex - firstIndex;

					if (count > 1)
					{
						logFormat.push_back(kString::format::g_SpecifierSymbol<char>);
						logFormat.append(kString::stringify::StringIntegral<char>(count));
					}

					logFormat.push_back('}');

					i += count;
				}
			}
		}
	public:
		inline static constexpr auto DetailSpecifier = '&';

	protected:
		std::unordered_map<LogLevel::Value, std::string> formatMap;

	private:
		static const std::unordered_map<char, std::string> LogFormatSpecifiersMap;
	};
}

