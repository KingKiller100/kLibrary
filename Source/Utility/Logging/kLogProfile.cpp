#include "pch.hpp"

#include "kLogProfile.hpp"

#include "kLogging.hpp"

namespace klib::kLogs
{
	LogProfile::LogProfile( std::weak_ptr<Logging> logSystem, const std::string_view& profileName, LogLevel lvl )
		: name( profileName )
		, level( lvl )
	{
		if ( logSystem.expired() )
			throw std::runtime_error( "Log system must be instantiated before creating a profile" );

		logSystem.lock()->Register( shared_from_this() );
	}

	std::string_view LogProfile::GetName() const noexcept
	{
		return name;
	}

	LogLevel LogProfile::GetLevel() const noexcept
	{
		return level;
	}

	void LogProfile::SetLevel( LogLevel lvl )
	{
		level = lvl;
	}
}
