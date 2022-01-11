#include "pch.hpp"
#include "kLogging.hpp"

#include "Destinations/kiLoggerDestination.hpp"
#include "kLogEntry.hpp"

#include "../FileSystem/kFileSystem.hpp"
#include "../String/kToString.hpp"
#include "../Debug/Exceptions/LoggingExceptions.hpp"


namespace klib::kLogs
{
	using namespace kString;

	std::string_view LogDispatcher::LogDestRef::GetName() const
	{
		return dest->GetName();
	}

	void LogDispatcher::LogDestRef::AddRaw( const LogMessage& message )
	{
		dest->AddRaw( message );
	}

	void LogDispatcher::LogDestRef::AddEntry( const LogEntry& entry )
	{
		dest->AddEntry( entry );
	}

	bool LogDispatcher::LogDestRef::IsOpen() const
	{
		return dest->IsOpen();
	}

	void LogDispatcher::LogDestRef::Open()
	{
		dest->Open();
	}

	void LogDispatcher::LogDestRef::Close()
	{
		dest->Close();
	}

	iLogDestination& LogDispatcher::LogDestRef::Ref() const
	{
		return *dest;
	}

	LogDispatcher::LogDestRef::LogDestRef( std::shared_ptr<iLogDestination> destination )
		: dest( destination )
	{}

	std::string_view LogDispatcher::LogProfileRef::GetName() const noexcept
	{
		return profile->GetName();
	}

	void LogDispatcher::LogProfileRef::SetLevel( LogLevel lvl ) const
	{
		profile->SetLevel( lvl );
	}

	void LogDispatcher::LogProfileRef::AddNewLine() const
	{
		profile->AddNewLine();
	}

	void LogDispatcher::LogProfileRef::AddRaw( std::string_view text ) const
	{
		profile->AddRaw( text );
	}

	void LogDispatcher::LogProfileRef::AddBanner( std::string_view text, std::string_view frontPadding, std::string_view backPadding, std::uint16_t paddingCount ) const
	{
		profile->AddBanner( text, frontPadding, backPadding, paddingCount );
	}

	void LogDispatcher::LogProfileRef::AddEntry( LogLevel lvl, std::string_view text ) const
	{
		profile->AddEntry( lvl, text );
	}

	LogDispatcher::LogProfileRef::LogProfileRef( std::shared_ptr<LogProfile> prof )
		: profile( prof )
	{ }

	LogDispatcher::LogDispatcher()
		: profiles()
		, destinations()
	{}

	LogDispatcher::~LogDispatcher()
	{
		Close();
		UnregisterAll();
	}

	LogDispatcher::LogProfileRef LogDispatcher::RegisterProfile( std::string_view name, LogLevel level )
	{
		if ( auto iter = std::ranges::find_if( profiles,
			[&name]( const decltype(profiles)::value_type& pfl )
			{
				return name == pfl->GetName();
			} ); iter != profiles.end() )
		{
			return LogProfileRef( *iter );
		}

		const auto profile = profiles.emplace_back( LogProfile::Create( name, level ) );
		profile->SetDispatcher( this );
		return LogProfileRef( profile );
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
