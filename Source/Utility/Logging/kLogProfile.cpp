#include "pch.hpp"

#include "kLogProfile.hpp"
#include "kLogging.hpp"
#include "../Debug/Exceptions/kExceptionWrapper.hpp"
#include "../Debug/Exceptions/LoggingExceptions.hpp"

namespace klib::kLogs
{
	LogProfile::LogProfile( const std::string_view& profileName, LogLevel lvl )
		: name( profileName )
		, level( lvl )
		, dispatcher( nullptr )
	{ }


	std::string_view LogProfile::GetName() const noexcept
	{
		return name;
	}

	void LogProfile::SetLevel( LogLevel lvl )
	{
		level = lvl;
	}

	LogLevel LogProfile::GetLevel() const noexcept
	{
		return level;
	}

	void LogProfile::AddNewLine()
	{
		AddRaw( "" );
	}

	void LogProfile::AddRaw( std::string_view text )
	{
		VerifyDispatcherSet();

		dispatcher->AddRaw( shared_from_this(), text );
	}

	void LogProfile::AddBanner( std::string_view text, std::string_view frontPadding, std::string_view backPadding, std::uint16_t paddingCount )
	{
		VerifyDispatcherSet();

		dispatcher->AddBanner( shared_from_this(), text, frontPadding, backPadding, paddingCount );
	}

	void LogProfile::AddEntry( LogLevel lvl, std::string_view text )
	{
		VerifyDispatcherSet();

		if ( !Loggable( lvl ) )
			return;

		dispatcher->AddEntry( shared_from_this(), text );
	}

	void LogProfile::VerifyDispatcherSet() const
	{
		if ( !dispatcher )
			throw kDebug::LoggingExceptions{ kString::ToString( "{0} cannot log before setting the dispatcher", GetName() ) };
	}

	bool LogProfile::Loggable( LogLevel lvl ) const
	{
		return level <= lvl;
	}

	void LogProfile::SetDispatcher( LogDispatcher* dispatcher )
	{
		this->dispatcher = dispatcher;
	}
}
