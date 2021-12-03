#include "pch.hpp"
#include "kLogging.hpp"

#include "Destinations/kiLoggerDestination.hpp"

#include "../FileSystem/kFileSystem.hpp"
#include "../String/kToString.hpp"


namespace klib::kLogs
{
	using namespace kString;
	using namespace kCalendar;

	LogDispatcher::LogDispatcher()
	{}

	LogDispatcher::~LogDispatcher()
	{
		Close();
	}

	void LogDispatcher::Register( LogProfile* profile )
	{
		if ( std::ranges::find_if( profiles,
			[&profile]( const decltype(profiles)::value_type& pfl )
			{
				return profile == pfl;
			} ) != profiles.end() )
		{
			throw std::runtime_error( "Registering profile that already exists" );
		}

		profiles.emplace_back( profile );
	}

	void LogDispatcher::Unregister( LogProfile* profile )
	{
		if ( const auto iter = std::ranges::find_if( profiles,
			[&profile]( const decltype(profiles)::value_type& pfl )
			{
				return profile == pfl;
			} ); iter != profiles.end() )
		{
			profiles.erase( iter );
			return;
		}

		throw std::runtime_error( "Unregistering profile that does not exists" );
	}

	void LogDispatcher::SetGlobalLevel( const LogLevel newMin ) noexcept
	{
		for ( auto& profile : profiles )
		{
			profile->SetLevel( newMin );
		}
	}

	void LogDispatcher::AddRaw( LogProfile* profile, std::string_view text )
	{
		AddLog( LogMessage( text ) );
	}

	void LogDispatcher::AddEntry( LogProfile* profile, const LogMessage& message )
	{
		AddLog( LogEntry(
			level,
			profile,
			message
		) );
	}

	void LogDispatcher::AddBanner(
		LogLevel lvl
		, LogProfile* profile
		, const LogMessage& message
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

		const auto text = front + message.text + back;

		const LogMessage banner( text, message );

		AddLog( LogEntry(lvl, p) );
	}

	void LogDispatcher::AddLog( const LogEntry& entry )
	{
		Flush( entry );
	}

	void LogDispatcher::AddLog( const LogMessage& message )
	{
		Flush( message );
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
				throw std::runtime_error( ToString( "Failed to open log destination: {0}", dest->GetName() ) );
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
