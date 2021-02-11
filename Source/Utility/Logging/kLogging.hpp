#pragma once

#include "kLogLevel.hpp"
#include "kLogEntry.hpp"
#include "kLogMessage.hpp"

#include "Destinations/kFileLogger.hpp"
#include "Destinations/kConsoleLogger.hpp"

#include "../../Template/kToImpl.hpp"

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace std {
	namespace filesystem {
		class path;
	}
}

namespace klib
{
	namespace kLogs
	{
		class iLoggerDestination;

		class Logging
		{
			ENUM_CLASS(LogDestType, std::uint8_t
				, FILE = 0
				, CONSOLE
			);

		public:
			using LogEntries = std::deque<LogEntry>;
			using LogDestList = std::vector<std::unique_ptr<iLoggerDestination>>;

		public:
			Logging(const std::string_view& directory, const std::string_view& filename
				, const std::string_view& extension
				, const std::string_view& name = "Logger");

			Logging(const std::filesystem::path& path, const std::string_view& name = "Logger");

			~Logging();

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
			 *		Toggles whether logs output to system to keep a local cache
			 */
			void SetCacheMode(const bool enable) noexcept;

			/**
			 * \brief
			 *		Gets the file logger
			 * \return
			 *		FileLogger ref
			 */
			FileLogger& GetFile();

			/**
			 * \brief
			 *		Gets the file logger
			 * \return
			 *		FileLogger ref
			 */
			const FileLogger& GetFile() const;

			/**
			 * \brief
			 *		Gets the console logger
			 * \return
			 *		ConsoleLogger ref
			 */
			ConsoleLogger& GetConsole();

			/**
			 * \brief
			 *		Gets the console logger
			 * \return
			 *		ConsoleLogger ref
			 */
			const ConsoleLogger& GetConsole() const;

			/**
			 * \brief
			 *		Returns user added log destination
			 * \tparam T
			 *		Derived iLoggerDestination type
			 * \param index
			 *		index of extra logger
			 * \return
			 *		T&
			 */
			template<typename T>
			USE_RESULT T& GetExtraDestination(size_t index)
			{
				return kTemplate::ToImpl<T>(*destinations[LogDestType::Count() + index]);
			}
			
			/**
			 * \brief
			 *		Returns user added log destination
			 * \tparam T
			 *		Derived iLoggerDestination type
			 * \param index 
			 *		index of extra logger
			 * \return
			 *		T&
			 */
			template<typename T>
			const T& GetExtraDestination(size_t index) const
			{
				return kTemplate::ToImpl<T>(*destinations[LogDestType::Count() + index]);
			}

			/**
			 * \brief 
			 *		Adds a new destination to send log entries
			 * \tparam T
			 *		Derived iLoggerDestination type
			 * \tparam Params
			 *		Parameters to derived type's ctor
			 * \param params 
			 */
			template<typename T, typename ...Params>
			void AddDestination(Params&& ...params)
			{
				T* dest = new T(std::forward<Params>(params)...);
				dest->SetName(&name);
				destinations.emplace_back(dest);
			}

			/**
			 * \brief
			 *		Outputs cached kLogs to file
			 */
			void FinalOutput(bool outputDefaultClosingMsg);

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
			void AddFatal(const LogMessage& msg);

			/**
			 * \brief
			 *		Logs text verbatim. No time, data or padding
			 * \param text
			 *		Text to log
			 */
			void AddRaw(const std::string_view& text = "");

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
			 *		If in cache mode, erases a maximum of count many of the most recent logs
			 * \param count
			 *		Number of logs to erase
			 * \return
			 *		TRUE if managed to erase any, FALSE if nothing erased or not in cache mode
			 */
			bool ErasePrevious(size_t count);

			/**
			 * \brief
			 *		Checks log level is above the minimum
			 * \param lvl
			 *		Log level of upcoming entry
			 * \return
			 *		TRUE if equal/above minimum log level
			 */
			bool IsLoggable(const LogLevel lvl) const;

			/**
			 * \brief
			 *		Check if cache is empty
			 * \return
			 *		TRUE if cache not empty OR FALSE if cache is empty
			 */
			bool HasCache() const noexcept;

		private:
			/**
			 * \brief
			 *		Opens log destinations
			 */
			void Open();

			void Initialize(const std::filesystem::path& path);

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
			 *		Outputs logs to file then closes it
			 */
			void Close(bool outputDefaultClosingMsg);

		protected:
			LogEntries entriesCache; // Queue buffer to cache the logged messages

			LogLevel minimumLoggingLevel;
			LogDestList destinations;
			std::string name;
			bool isEnabled;

			const bool g_EnableDebugLogging =
#ifdef KLOG_OPT_DBG_STR
				true
#else
				false
#endif
				;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif
}
