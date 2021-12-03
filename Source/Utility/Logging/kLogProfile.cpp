#include "pch.hpp"

#include "kLogProfile.hpp"

#include "kLogging.hpp"

namespace klib::kLogs
{
	LogProfile::LogProfile( std::weak_ptr<LogDispatcher> dispatcher, const std::string_view& profileName, LogLevel lvl )
		: name( profileName )
		, level( lvl )
		, dispatcher( dispatcher )
	{
		if ( dispatcher.expired() )
			throw std::runtime_error( "Log dispatcher must be instantiated before creating a profile to register" );

		dispatcher.lock()->Register( this );
	}

	LogProfile::~LogProfile()
	{
		if ( dispatcher.expired() )
			throw std::runtime_error( "Cannot unregister a profile after destroying the dispatcher" );

		dispatcher.lock()->Unregister( this );
	}

	std::string_view LogProfile::GetName() const noexcept
	{
		return name;
	}

	void LogProfile::SetLevel( LogLevel lvl )
	{
		level = lvl;
	}

	void LogProfile::AddRaw( std::string_view text )
	{
		if ( dispatcher.expired() )
			throw std::runtime_error( "Cannot raw log to an empty dispatcher" );

		dispatcher.lock()->AddRaw( this, text );
	}

	void LogProfile::AddBanner( const LogMessage& message, std::string_view frontPadding, std::string_view backPadding, std::uint16_t paddingCount )
	{
		if ( dispatcher.expired() )
			throw std::runtime_error( "Cannot add a banner to a empty dispatcher" );

		dispatcher.lock()->AddBanner( , this, message, frontPadding, backPadding, paddingCount );
	}

	void LogProfile::AddEntry( LogLevel lvl, const LogMessage& message )
	{
		if ( dispatcher.expired() )
			throw std::runtime_error( "Cannot add a log entry to an empty dispatcher" );

		if ( !Loggable( lvl ) )
			return;

		dispatcher.lock()->AddEntry( this, message );
	}

	bool LogProfile::Loggable( LogLevel lvl ) const
	{
		return level <= lvl;
	}
}
