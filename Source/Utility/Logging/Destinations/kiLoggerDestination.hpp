#pragma once

#include "../../String/Format/Stringify/kStringifyInteger.hpp"

#include <string>
#include <unordered_map>

namespace klib::kLogs
{
	struct LogMessage;
	class LogEntry;

	class iLogDestination
	{
	public:
		virtual ~iLogDestination() = default;

		/**
		 * \brief
		 *		Returns name of logger destination
		 * \return
		 *		Destination's name
		 */
		virtual std::string_view GetName() const = 0;

		/**
		 * \brief
		 *		Writes log to destination without formatting it
		 * \param message
		 *		Message
		 */
		virtual void AddRaw( const LogMessage& message ) = 0;

		/**
		 * \brief
		 *		Formats log message and level to the appropriate log message and then caches it
		 * \param[in] entry
		 *		Log entry containing message, time, date, log level, and profile
		 */
		virtual void AddEntry( const LogEntry& entry ) = 0;

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
		virtual void Open() = 0;

		/**
		 * \brief
		 *		Closes the log destination
		 */
		virtual void Close() = 0;
	};

	struct FormattedLogDestinationBase : public iLogDestination
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
		 *		- d = Day
		 *		- m = Month
		 *		- y = Year
		 *		- h = Hours
		 *		- z = Minutes
		 *		- s = Seconds
		 *		- c = Milliseconds
		 *		- n = Profile name
		 *		- p = Log descriptor [text]
		 *		- w = Log descriptor [numeric value]
		 *		- t = Log message
		 */
		virtual void SetFormat( const std::string_view& format ) noexcept;
	public:
		inline static constexpr auto DetailSpecifier = '&';

	protected:
		std::string messageFormat;

	private:
		static const std::unordered_map<char, std::string> LogFormatSpecifiersMap;
	};
}
