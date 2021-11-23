#pragma once

#include <string>

namespace klib::kLogs
{
	class LogProfile
	{
	public:
		LogProfile( const std::string_view& profileName );

		[[nodiscard]] std::string_view GetName() const noexcept;

	private:
		std::string name;
	};
}
