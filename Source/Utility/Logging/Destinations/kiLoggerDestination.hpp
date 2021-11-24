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
		 *		Returns name of logger destination
		 * \return
		 *		Destination's name
		 */
		virtual std::string_view GetName() const = 0;

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
		virtual void Close() = 0;
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
		virtual void SetFormat( const LogLevel lvl, const std::string_view& format ) noexcept;
	public:
		inline static constexpr auto DetailSpecifier = '&';

	protected:
		std::unordered_map<LogLevel::Value, std::string> formatMap;

	private:
		static const std::unordered_map<char, std::string> LogFormatSpecifiersMap;
	};
}

