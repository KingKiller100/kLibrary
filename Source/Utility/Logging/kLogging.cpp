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

	LogProfile* LogProfileRef::operator->() const
	{
		return profile_.get();
	}

	bool LogProfileRef::IsNull() const noexcept
	{
		return profile_ == nullptr;
	}

	LogProfileRef::LogProfileRef( std::shared_ptr<LogProfile> prof )
		: profile_( prof )
	{}

	LogDispatcher::LogDispatcher()
		: profiles_()
		, destinations_()
	{}

	LogDispatcher::~LogDispatcher()
	{
		Close();
		UnregisterAll();
	}

	LogProfileRef LogDispatcher::RegisterProfile( std::string_view name, LogLevel level )
	{
		if ( auto iter = std::ranges::find_if( profiles_,
			[&name]( const decltype(profiles_)::value_type& pfl )
			{
				return name == pfl->GetName();
			} ); iter != profiles_.end() )
		{
			return LogProfileRef( *iter );
		}

		const auto profile = profiles_.emplace_back( LogProfile::Create( name, level ) );
		profile->SetDispatcher( this );
		return LogProfileRef( profile );
	}

	void LogDispatcher::UnregisterAll()
	{
		for ( auto& logProfile : profiles_ )
		{
			profiles_.clear();
		}
	}

	void LogDispatcher::SetGlobalLevel( LogLevel newMin ) noexcept
	{
		for ( const auto& profile : profiles_ )
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
		for ( auto& dest : destinations_ )
		{
			dest->AddEntry( entry );
		}
	}

	void LogDispatcher::Flush( const LogMessage& message )
	{
		for ( auto& dest : destinations_ )
		{
			dest->AddRaw( message );
		}
	}

	void LogDispatcher::Open()
	{
		for ( auto& dest : destinations_ )
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
		for ( auto& dest : destinations_ )
		{
			dest->Close();
		}
	}
}
