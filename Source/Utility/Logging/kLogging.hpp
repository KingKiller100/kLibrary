#pragma once

#include "kLogLevel.hpp"
#include "kLogEntry.hpp"
#include "kLogProfile.hpp"
#include "kLogMessage.hpp"

#include <cstdint>
#include <deque>
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
		class iLogDestination;

		class Logging
		{
		public:
			Logging();

			~Logging();

			/**
			 * \brief
			 *		Set minimum log level for all profiles
			 * \param newMinLevel
			 *		New min log level
			 */
			void SetGlobalLevel( const LogLevel newMinLevel ) noexcept;

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
			 * \param entry
			 *		Log Entry
			 */
			void Flush( const LogEntry& entry );
			
			/**
			 * \brief
			 *		Flush message to destination
			 * \param message
			 *		Log message
			 */
			void Flush( const LogMessage& message );

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
			

		private:

			/**
			 * \brief
			 *		outputs entry to destination
			 * \param entry
			 *		Log entry
			 * \param entry
			 *		Log description
			 */
			void AddLog( const LogEntry& entry );
			
			/**
			 * \brief
			 *		Outputs message to destination
			 * \param message
			 *		Log Message
			 */
			void AddLog( const LogMessage& message );

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
			// std::deque<LogEntry> entriesCache; // Queue buffer to cache the logged messages

			std::vector<std::shared_ptr<LogProfile>> profiles;
			std::vector<std::shared_ptr<iLogDestination>> destinations;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif
}
