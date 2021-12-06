#include "pch.hpp"
#include "kLogging.hpp"

#include "Destinations/kiLoggerDestination.hpp"

#include "../FileSystem/kFileSystem.hpp"
#include "../String/kToString.hpp"
#include "../Debug/Exceptions/LoggingExceptions.hpp"


namespace klib::kLogs
{
	using namespace kString;
	using namespace kCalendar;

	LogDispatcher::LogDispatcher()
		: profiles()
		, destinations()
	{}

	LogDispatcher::~LogDispatcher()
	{
		Close();
		UnregisterAll();
	}

	std::weak_ptr<LogProfile> LogDispatcher::Register( std::string_view name, LogLevel level )
	{
		if ( auto iter = std::ranges::find_if( profiles,
			[&name]( const decltype(profiles)::value_type& pfl )
			{
				return name == pfl->GetName();
			} ); iter != profiles.end() )
		{
			return *iter;
		}

		const auto profile = profiles.emplace_back( std::make_shared<LogProfile>( name, level ) );
		profile->SetDispatcher( this );
		return profile;
	}

	void LogDispatcher::UnregisterAll()
	{
		for ( auto& logProfile : profiles )
		{
			profiles.clear();
		}
	}

	void LogDispatcher::SetGlobalLevel( LogLevel newMin ) noexcept
	{
		for ( const auto& profile : profiles )
		{
			profile->SetLevel( newMin );
		}
	}

	void LogDispatcher::AddRaw( std::shared_ptr<LogProfile>, std::string_view text )
	{
		AddLog( text );
	}

	void LogDispatcher::AddEntry( std::shared_ptr<LogProfile> profile, std::string_view message )
	{
		AddLog( LogEntry(
			profile,
			LogMessage( message )
		) );
	}

	void LogDispatcher::AddBanner(
		std::shared_ptr<LogProfile> profile
		, std::string_view message
		, std::string_view frontPadding
		, std::string_view backPadding
		, std::uint16_t paddingCount
	)
	{
		std::string front, back;
		for ( auto i = 0; i < paddingCount; ++i )
		{
			front.append( frontPadding );
			back.append( backPadding );
		}

		const auto text = front + std::string( message ) + back;

		const LogMessage banner( text );

		AddLog( LogEntry( profile, banner ) );
	}

	void LogDispatcher::AddLog( const LogEntry& entry )
	{
		Flush( entry );
	}

	void LogDispatcher::AddLog( std::string_view text )
	{
		Flush( LogMessage( text ) );
	}

	void LogDispatcher::Flush( const LogEntry& entry )
	{
		for ( auto& dest : destinations )
		{
			dest->AddEntry( entry );
		}
	}

	void LogDispatcher::Flush( const LogMessage& message )
	{
		for ( auto& dest : destinations )
		{
			dest->AddRaw( message );
		}
	}

	void LogDispatcher::Open()
	{
		for ( auto& dest : destinations )
		{
			dest->Open();
			if ( !dest->IsOpen() )
			{
				throw kDebug::LoggingExceptions( ToString( "Failed to open log destination: {0}", dest->GetName() ) );
			}
		}
	}

	void LogDispatcher::Close()
	{
		for ( auto& dest : destinations )
		{
			dest->Close();
		}
	}
}
