#include "pch.hpp"

#include "kLogProfile.hpp"

namespace klib::kLogs
{
	LogProfile::LogProfile( const std::string_view& profileName )
		: name( profileName )
	{ }

	std::string_view LogProfile::GetName() const noexcept
	{
		return name;
	}
}
