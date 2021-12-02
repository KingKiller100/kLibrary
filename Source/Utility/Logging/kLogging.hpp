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
			Logging();

			~Logging();

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
			void SetLevel( std::shared_ptr<LogProfile> profile, LogLevel newMinLevel ) noexcept;

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
			std::shared_ptr<T>& AddDestination( Params&& ...params )
			{
				return destinations.emplace_back( std::make_shared<T>(std::forward<Params>(params)...) );
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
			void AddRaw( std::string_view text = "" );

			/**
			 * \brief
			 *		Formats log message and level to the appropriate log message and then logs it
			 * \param message
			 *		Log message details including time, data, text, source file and source line
			 * \param profile
			 *		Source of log
			 * \param level
			 *		Log entry details
			 */
			void AddEntry( LogLevel level, const LogProfile& profile, const LogMessage& message );

			/**
			 * \brief
			 *		Formats the log banner to become the appropriate log banner message and logs it
			 * \param message
			 *		Log message details including time, data, text, source file and source line
			 * \param frontPadding
			 *		Padding character/string before banner text
			 * \param backPadding
			 *		Padding character/string after banner text
			 * \param paddingCount
			 *		Repetition of paddings
			 */
			void AddBanner(
				const LogMessage& message
				, std::string_view frontPadding
				, std::string_view backPadding
				, std::uint16_t paddingCount
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

			/**
			 * \brief
			 *		Registers a profile with a logging system
			 * \param profile
			 *		profile
			 * \param level
			 *		Initial lvl
			 */
			void Register( std::shared_ptr<LogProfile> profile );

			friend class LogProfile;

		protected:
			std::deque<LogEntry> entriesCache; // Queue buffer to cache the logged messages

			std::vector<std::shared_ptr<LogProfile>> profiles;
			std::vector<std::shared_ptr<iLoggerDestination>> destinations;

			bool outputEnabled;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif
}
