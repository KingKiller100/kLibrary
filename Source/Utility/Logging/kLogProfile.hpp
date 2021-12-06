﻿#pragma once

#include "kLogLevel.hpp"
#include <string>

namespace klib::kLogs
{
	struct LogMessage;
	class LogDispatcher;

	class LogProfile : public std::enable_shared_from_this<LogProfile>
	{
	public:
		explicit LogProfile(const std::string_view& profileName, LogLevel lvl);
		LogProfile( LogProfile&& ) = default;
		LogProfile& operator=( LogProfile&& ) = default;

		[[nodiscard]] std::string_view GetName() const noexcept;

		void SetLevel( LogLevel lvl );
		LogLevel GetLevel() const noexcept;

		void AddNewLine();

		/**
		 * \brief
		 *		Logs text verbatim. No time, data or padding
		 * \param text
		 *		Text to log
		 */
		void AddRaw( std::string_view text );

		/**
		 * \brief
		 *		Formats the log banner to become the appropriate log banner message and logs it
		 * \param text
		 *		Log message details including time, data, text, source file and source line
		 * \param frontPadding
		 *		Padding character/string before banner text
		 * \param backPadding
		 *		Padding character/string after banner text
		 * \param paddingCount
		 *		Repetition of paddings
		 */
		void AddBanner(
			std::string_view text
			, std::string_view frontPadding
			, std::string_view backPadding
			, std::uint16_t paddingCount
		);

		/**
		 * \brief
		 *		Formats log message and level to the appropriate log message and then logs it
		 * \param text
		 *		Log message details including time, data, text, source file and source line
		 * \param lvl
		 *		Log entry details
		 */
		void AddEntry( LogLevel lvl, std::string_view text );

		void VerifyDispatcherSet() const;

		friend class LogDispatcher;

	private:
		bool Loggable( LogLevel lvl ) const;

		void SetDispatcher( LogDispatcher* dispatcher );

	private:
		std::string name;
		LogLevel level;
		LogDispatcher* dispatcher;
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
