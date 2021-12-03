#pragma once

#include "kLogLevel.hpp"
#include <string>

namespace klib::kLogs
{
	struct LogMessage;
	class LogDispatcher;

	class LogProfile
	{
	public:
		explicit LogProfile( std::weak_ptr<LogDispatcher> dispatcher, const std::string_view& profileName, LogLevel lvl = LogLevel::INF );

		LogProfile(LogProfile&&) = default;
		LogProfile& operator=(LogProfile&&) = default;

		~LogProfile();

		[[nodiscard]] std::string_view GetName() const noexcept;
		
		void SetLevel( LogLevel lvl );

		/**
		 * \brief
		 *		Logs text verbatim. No time, data or padding
		 * \param text
		 *		Text to log
		 */
		void AddRaw(std::string_view text = "");

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
		 *		Formats log message and level to the appropriate log message and then logs it
		 * \param message
		 *		Log message details including time, data, text, source file and source line
		 * \param lvl
		 *		Log entry details
		 */
		void AddEntry(LogLevel lvl, const LogMessage& message);

	private:
		bool Loggable(LogLevel lvl) const;

	private:
		std::string name;
		LogLevel level;
		std::weak_ptr<LogDispatcher> dispatcher;
	};
}

namespace std
{
	// STRUCT TEMPLATE SPECIALIZATION hash
	template <>
	struct hash<klib::kLogs::LogProfile>
	{
		_NODISCARD size_t operator()( const klib::kLogs::LogProfile& profile ) const noexcept
		{
			return hash<std::string_view>{}( profile.GetName() );
		}
	};
}
