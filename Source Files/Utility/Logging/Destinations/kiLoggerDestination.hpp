#pragma once

#include "../../../HelperMacros.hpp"
#include "../../Enum/kEnum.hpp"

#include <string>

namespace klib::kLogs
{
	class LogEntry;
	class LogDescriptor;

	class iLoggerDestination
	{
	public:
		inline static constexpr auto DetailSpecifier = '&';

		ENUM_CLASS(FormatIndex, char,
			dayIndex = '0',
			monthIndex = '1',
			yearIndex = '2',

			hourIndex = '3',
			minuteIndex = '4',
			secondIndex = '5',
			millisecondIndex = '6',

			nameIndex = '7',

			messageIndex = '8'
			);

	public:
		virtual ~iLoggerDestination() = default;

		/**
		 * \brief
		 *		Returns the name of the logger
		 * \return
		 *		Returns name as a string_view
		 */
		USE_RESULT virtual std::string_view GetName() const = 0;

		/**
		 * \brief
		 *		Change name of the logger
		 * \param[in] newName
		 *		STL string representing a name
		 */
		virtual void SetName(const std::string_view& newName) = 0;

		/**
		 * \brief
		 *		Sets the format of all log message
		 *		[Example] "[&dd/&mm/&yyyy] [&hh:&zz:&ss] [&n]: &t"
		 *		means "[01/01/1970] [01:12:59] [Logger]: Pass Test!
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
		 *		- t/t = Text
		 */
		virtual void SetFormat(const std::string_view& format) noexcept = 0;

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
		virtual bool IsOpen() = 0;

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
}

