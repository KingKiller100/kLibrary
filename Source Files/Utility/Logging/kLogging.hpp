#pragma once

#include "kLogLevel.hpp"
#include "kLogEntry.hpp"
#include "kLogMessage.hpp"

#include <unordered_map>
#include <deque>
#include <string>
#include <cstdint>

namespace klib
{
	namespace kLogs
	{
		class iLoggerDestination;
		
		class Logging
		{
			ENUM_CLASS(DestionationType , std::uint8_t
				, FILE
				, CONSOLE);
			
		public:
			using LogEntries = std::deque<LogEntry>;
			using LogDestinationsMap = std::unordered_map<DestionationType::enum_t
			, std::unique_ptr<iLoggerDestination>>;

		public:
			Logging(const std::string_view& directory, const std::string_view& filename
				, const std::string_view& extension = ".log"
				, const std::string_view& name = "Logger");

			~Logging();

			/**
			 * \brief
			 *		Initializes logging system
			 * \param[in] openingMsg
			 *		Opening message
			 * \note
			 *		No logging calls will function properly until this is called.
			 */
			void OutputInitialized(const std::string_view& openingMsg);

			/**
			 * \brief
			 *		Change name of the logger
			 * \param[in] newName
			 *		STL string representing a name
			 */
			void SetName(const std::string_view& newName);

			/**
			 * \brief
			 *		Set minimum level of a log that can be stored
			 * \param[in] newMinLevel
			 *		New minimum log level
			 * \note
			 *		No logs less than this given level will be stored by the log system.
			 */
			void SetMinimumLoggingLevel(const LogLevel newMinLevel) noexcept;

			/**
			 * \brief
			 *		Toggles if logging system is enabled
			 */
			constexpr void ToggleLoggingEnabled() noexcept;

			/**
			 * \brief
			 *		Toggles if sub system outputting is enabled
			 */
			void ToggleConsoleEnabled() noexcept;

			/**
			 * \brief
			 *		Toggles whether logs output to system to keep a local cache
			 */
			constexpr void SetCacheMode(const bool enable) noexcept;

			/**
			 * \brief
			 *		Change the directory the log file outputs to
			 * \param dir
			 *		STL string view representing the new directory
			 * \param filename
			 *		STL string view representing the new filename
			 */
			void ChangeOutputPath(const std::string_view& dir, const std::string_view& filename);

			/**
			 * \brief
			 *		Change the directory the log file outputs to
			 * \param directory
			 *		STL string view representing the new directory
			 */
			void ChangeOutputDirectory(const std::string_view& directory);

			/**
			 * \brief
			 *		Change name of log filename
			 * \param filename
			 *		STL string view representing filename
			 */
			void ChangeFilename(const std::string_view& filename);

			/**
			 * \brief
			 *		Gets the full output path of the logger
			 * \return
			 *		Gets the full output path of the logger
			 */
			std::string GetOutputPath() const;

			/**
			 * \brief
			 *		Appends the current log file with the current logging cached in the buffer
			 *		and pauses outputing to log file.
			 * \note
			 *		Allows user to keep logging but just keeps it in cache and outputs to subsystems
			 */
			void SuspendFileLogging();

			/**
			 * \brief
			 *		Continues logging and flushes cache to file
			 */
			void ResumeFileLogging();

			/**
			 * \brief
			 *		Outputs cached kLogs to file
			 */
			void FinalOutput();

			/**
			 * \brief
			 *		Flush stored log stream to file
			 */
			void Flush();

			/**
			 * \brief
			 *		Outputs all cached kLogs up to file with the logged error message at the end
			 * \param msg
			 *		Error information
			 */
			void OutputToFatalFile(const LogMessage& msg);

			/**
			 * \brief
			 *		Logs text verbatim. No time, data or padding
			 * \param text
			 *		Text to log
			 */
			void AddVerbatim(const std::string_view& text = "");
			
			/**
			 * \brief
			 *		Formats log message and level to the appropriate log message and then logs it
			 * \param message
			 *		Log message details including time, data, text, source file and source line
			 * \param level
			 *		Log entry details
			 */
			void AddEntry(const LogLevel& level, const LogMessage& message);

			/**
			 * \brief
			 *		Formats the log banner to become the appropriate log banner message and logs it
			 * \param message
			 *		Log message details including time, data, text, source file and source line
			 * \param[in] descriptor
			 *		Log descriptor of lvl and type info
			 * \param frontPadding
			 *		Padding character/string before banner text
			 * \param backPadding
			 *		Padding character/string after banner text
			 * \param paddingCount
			 *		Repetition of paddings
			 */
			void AddBanner(const std::string_view& descriptor, const LogMessage& message, const std::string_view& frontPadding, const std::
			               string_view& backPadding, const std::uint16_t paddingCount);

			/**
			 * \brief
			 *		Returns previous entry text
			 *
			 * \return
			 *		String of the final log entry
			*/
			const LogEntry& GetLastCachedEntry() const;

			/**
			 * \brief
			 *		Deletes all log entries
			*/
			void ClearCache();

			/**
			 * \brief
			 *		Enables constant flushing
			 * \param enable
			 *		Boolean representing whether to constantly flush or not
			 */
			void EnableConstantFlush(bool enable);
			
			/**
			 * \brief
			 *		If in cache mode, erases a maximum of count many of the most recent logs
			 * \param count
			 *		Number of logs to erase
			 * \return
			 *		TRUE if managed to erase any, FALSE if nothing erased or not in cache mode
			 */
			bool ErasePrevious(size_t count);

		private:
			void Initialize(const std::string_view& directory, const std::string_view& filename, const std::string_view& extension);

			/**
			 * \brief
			 *		Adds an entry and log description to queue
			 * \param entry
			 *		Log entry
			 * \param entry
			 *		Log description
			 */
			void AddLog(const LogEntry& entry);
			
			/**
			 * \brief
			 *		Opens log destinations
			 */
			void Open();
			
			/**
			 * \brief
			 *		Outputs logs to file then closes it
			 */
			void Close();

			/**
			 * \brief
			 *		Checks log level is above the minimum
			 * \param lvl
			 *		Log level of upcoming entry
			 * \return
			 *		TRUE if equal/above minimum log level
			 */
			bool IsLoggable(const LogLevel lvl) const;
			
		protected:
			LogEntries entriesQ; // Queue buffer to cache the logged messages

			LogLevel minimumLoggingLevel;
			LogDestinationsMap destinations;
			std::string name;
			bool isEnabled;
			bool cacheMode;
			bool constantFlushing;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif
}
