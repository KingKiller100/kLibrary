#pragma once

#include "kLogLevel.hpp"
#include <string>

namespace klib::kLogs
{
	class Logging;

	class LogProfile : std::enable_shared_from_this<LogProfile>
	{
	public:
		explicit LogProfile( std::weak_ptr<Logging> logSystem, const std::string_view& profileName, LogLevel lvl = LogLevel::INF );

		[[nodiscard]] std::string_view GetName() const noexcept;

		[[nodiscard]] LogLevel GetLevel() const noexcept;

		void SetLevel( LogLevel lvl );

		auto operator<=>( const LogProfile& ) const = default;

	private:
		std::string name;
		LogLevel level;
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
