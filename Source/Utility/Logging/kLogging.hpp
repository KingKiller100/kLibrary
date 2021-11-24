#pragma once

#include "kLogLevel.hpp"
#include "kLogEntry.hpp"
#include "kLogProfile.hpp"
#include "kLogMessage.hpp"

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace std
{
	namespace filesystem
	{
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
		public:
			// using LogEntries = std::deque<LogEntry>;
			// using LogDestList = std::vector<std::unique_ptr<iLoggerDestination>>;

		public:
			Logging();

			~Logging();

			void Register( const LogProfile& profile, LogLevel level );

			/**
			 * \brief
			 *		Set minimum level of a log that can be stored
			 * \param[in] profile
			 *		Log profile
			 * \param[in] newMinLevel
			 *		New minimum log level
			 * \note
			 *		No logs less than this given level will be stored by the log system.
			 */
			void SetLevel( const LogProfile& profile, LogLevel newMinLevel ) noexcept;

			/**
			 * \brief
			 *		Set minimum log level for all profiles
			 * \param newMinLevel
			 *		New min log level
			 */
			void SetGlobalLevel( const LogLevel newMinLevel ) noexcept;

			/**
			 * \brief
			 *		Get a profile's log level
			 * \param profile
			 *		Log level of the given profile
			 * \return
			 *		Profile's current log level
			 */
			LogLevel GetLevel( const LogProfile& profile ) const;

			/**
			 * \brief
			 *		Toggles if logging system is enabled
			 */
			void EnableOutput( bool enabled ) noexcept;

			/**
			 * \brief
			 *		Toggles whether logs output to system to keep a local cache
			 */
			void SetCacheMode( const bool caching ) noexcept;

			/**
			 * \brief 
			 *		Adds a new destination to send log entries
			 * \tparam T
			 *		Derived iLoggerDestination type
			 * \tparam Params
			 *		Parameters to derived type's ctor
			 * \param params 
			 */
			template <typename T, typename ...Params>
			void AddDestination( Params&& ...params )
			{
				T* dest = new T( std::forward<Params>( params )... );
				dest->SetName( &name );
				destinations.emplace_back( dest );
			}

			/**
			 * \brief
			 *		Flush stored log stream to file
			 */
			void Flush();

			/**
			 * \brief
			 *		Logs text verbatim. No time, data or padding
			 * \param text
			 *		Text to log
			 */
			void AddRaw( const std::string_view& text = "" );

			/**
			 * \brief
			 *		Formats log message and level to the appropriate log message and then logs it
			 * \param message
			 *		Log message details including time, data, text, source file and source line
			 * \param level
			 *		Log entry details
			 */
			void AddEntry( const LogProfile& profile, const LogLevel& level, const LogMessage& message );

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
			void AddBanner(
				const std::string_view& descriptor
				, const LogMessage& message
				, const std::string_view& frontPadding
				, const std::
				string_view& backPadding
				, const std::uint16_t paddingCount
			);

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
			bool ErasePrevious( size_t count );

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
			 *		Initialize logging system
			 */
			void Initialize();

			/**
			 * \brief
			 *		Adds an entry and log description to queue
			 * \param entry
			 *		Log entry
			 * \param entry
			 *		Log description
			 */
			void AddLog( const LogEntry& entry );

			/**
			 * \brief
			 *		Open log destinations
			 */
			void Open();

			/**
			 * \brief
			 *		Outputs logs to file then closes it
			 */
			void Close();

		protected:
			std::deque<LogEntry> entriesCache; // Queue buffer to cache the logged messages

			std::unordered_map<LogProfile, LogLevel> logLevels;
			std::vector<std::shared_ptr<iLoggerDestination>> destinations;

			bool outputEnabled;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif
}
