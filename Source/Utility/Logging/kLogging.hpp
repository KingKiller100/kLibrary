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

		class LogDispatcher
		{
		public:
			LogDispatcher();

			~LogDispatcher();

			/**
			 * \brief
			 *		Set minimum log level for all profiles
			 * \param newMinLevel
			 *		New min log level
			 */
			void SetGlobalLevel( const LogLevel newMinLevel ) noexcept;

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
			void AddRaw( LogProfile* profile, std::string_view text = "" );

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
			void AddEntry( LogProfile* profile, const LogMessage& message );

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
				LogProfile* profile
				, const LogMessage& message
				, std::string_view frontPadding
				, std::string_view backPadding
				, std::uint16_t paddingCount
			);

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
			std::shared_ptr<T>& AddDestination(Params&& ...params)
			{
				return destinations.emplace_back(std::make_shared<T>(std::forward<Params>(params)...));
			}

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
			 */
			void Register( LogProfile* profile );
			void Unregister( LogProfile* profile );

			friend class LogProfile;

		protected:
			std::vector<LogProfile*> profiles;
			std::vector<std::shared_ptr<iLogDestination>> destinations;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif
}
