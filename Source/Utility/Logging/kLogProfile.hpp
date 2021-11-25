#pragma once

#include <string>

namespace klib::kLogs
{
	class LogProfile
	{
	public:
		explicit LogProfile( const std::string_view& profileName );

		[[nodiscard]] std::string_view GetName() const noexcept;

		auto operator<=>(const LogProfile&) const = default;

	private:
		std::string name;
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
