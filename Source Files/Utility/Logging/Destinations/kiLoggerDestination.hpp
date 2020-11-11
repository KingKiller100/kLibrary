#pragma once

#include "../../../HelperMacros.hpp"
#include <string>

namespace klib::kLogs
{
	class LogEntry;
	class LogDescriptor;

	class iLoggerDestination
	{
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
		 *		Output message when logging system is initialized
		 * \param openingMsg
		 *		Opening message 
		 */
		virtual void OutputInitialized(const std::string_view& openingMsg) = 0;

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

