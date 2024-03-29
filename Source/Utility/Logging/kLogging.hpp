#pragma once

#include "kLogProfile.hpp"
#include "kLogLevel.hpp"

#include <cstdint>
#include <vector>

#include "Destinations/kiLoggerDestination.hpp"

namespace klib
{
	namespace kLogs
	{
		class LogEntry;

		class LogDispatcher
		{
		public:
			template <typename T, typename = std::enable_if_t<std::is_base_of_v<iLogDestination, T>>>
			class LogDestRef
			{
			public:
				LogDestRef() noexcept = default;

				T* operator->() const
				{
					return dest_.operator->();
				}

				[[nodiscard]] bool IsNull() const
				{
					return dest_ == nullptr;
				}

				friend class LogDispatcher;

			private:
				explicit LogDestRef( std::shared_ptr<iLogDestination> destination )
					: dest_( std::dynamic_pointer_cast<T>( destination ) )
				{ }

			private:
				std::shared_ptr<T> dest_;
			};

		public:
			LogDispatcher();

			~LogDispatcher();

			/**
			 * \brief
			 *		Set minimum log level for all profiles
			 * \param newMinLevel
			 *		New min log level
			 */
			void SetGlobalLevel( LogLevel newMinLevel ) noexcept;

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
			LogDestRef<T> AddDestination( Params&& ...params )
			{
				auto destination = std::make_shared<T>( std::forward<Params>( params )... );
				destinations_.emplace_back( destination );
				return LogDestRef<T>( destination );
			}

			/**
			 * \brief
			 *		Registers a profile with a logging system
			 * \param name
			 *		profile
			 */
			LogProfileRef RegisterProfile( std::string_view name, LogLevel level );

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

		private:
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
			void AddRaw( std::shared_ptr<LogProfile> profile, std::string_view text = "" );

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
			void AddEntry( std::shared_ptr<LogProfile> profile, std::string_view message );

			/**
			 * \brief
			 *		Formats the log banner to become the appropriate log banner message and logs it
			 * \param profile
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
				std::shared_ptr<LogProfile> profile
				, std::string_view message
				, std::string_view frontPadding
				, std::string_view backPadding
				, std::uint16_t paddingCount
			);

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
			 * \param text
			 *		Log Message
			 */
			void AddLog( std::string_view text );

			/**
			 * \brief
			 *		Unregisters all previously registered profiles
			 */
			void UnregisterAll();

			friend class LogProfile;

		protected:
			std::vector<std::shared_ptr<LogProfile>> profiles_;
			std::vector<std::shared_ptr<iLogDestination>> destinations_;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif
}
