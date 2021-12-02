#include "pch.hpp"
#include "kLogging.hpp"

#include "Destinations/kiLoggerDestination.hpp"

#include "../FileSystem/kFileSystem.hpp"
#include "../String/kToString.hpp"


namespace klib::kLogs
{
	using namespace kString;
	using namespace kCalendar;

	Logging::Logging()
		: outputEnabled( false )
	{
		Initialize();
	}

	Logging::~Logging()
	{
		Flush();
		Close();
	}

	void Logging::Register( std::shared_ptr<LogProfile> profile )
	{
		if ( std::ranges::find_if( profiles,
			[&profile](const decltype(profiles)::value_type& pfl)
			{
			return profile == pfl;
		}) != profiles.end())
		{
			throw std::runtime_error("Registering profile that already exists");
		}

		profiles.emplace_back( profile );
	}

	void Logging::SetLevel( std::shared_ptr<LogProfile> profile, LogLevel newMinLevel ) noexcept
	{
		if (const auto iter = std::ranges::find_if(profiles,
			[&profile](const decltype(profiles)::value_type& pfl)
		{
			return profile == pfl;
		}); 
			iter != profiles.end())
		{
		(*iter)->SetLevel( newMinLevel );
			return;
		}
	}

	void Logging::Initialize()
	{
		EnableOutput( true );
	}

	void Logging::EnableOutput( bool enabled ) noexcept
	{
		outputEnabled = enabled;
	}

	void Logging::SetGlobalLevel( const LogLevel newMin ) noexcept
	{
		for ( auto& [_, level] : profiles )
		{
			level = newMin;
		}
	}

	void Logging::SetCacheMode( const bool caching ) noexcept
	{
		if ( caching )
		{
			Close();
		}
		else
		{
			Open();
			Flush();
		}
	}

	void Logging::AddRaw( std::string_view text )
	{
		AddLog( LogEntry( LogLevel::NRM, LogProfile( "" ), LogMessage( text ) ) );
	}

	void Logging::AddEntry( LogLevel level, const LogProfile& profile, const LogMessage& message )
	{
		if ( GetLevel( profile ) > level )
			return;

		AddLog( LogEntry(
			level,
			profile,
			message
		) );
	}

	void Logging::AddBanner(
		const LogMessage& message
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

		AddLog( LogEntry( LogLevel::NRM, LogProfile( "" ), banner ) );
	}

	void Logging::AddLog( const LogEntry& entry )
	{
		entriesCache.push_back( entry );

		if ( !outputEnabled )
			return;

		Flush();
	}

	void Logging::Flush()
	{
		while ( !entriesCache.empty() )
		{
			const auto& entry = entriesCache.front();
			for ( auto& dest : destinations )
			{
				dest->AddEntry( entry );
			}
			entriesCache.pop_front();
		}
	}

	void Logging::Open()
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

	void Logging::Close()
	{
		for ( auto& dest : destinations )
		{
			dest->Close();
		}
	}

	LogLevel Logging::GetLevel( const LogProfile& profile ) const
	{
		return profiles.at( profile );
	}

	bool Logging::HasCache() const noexcept
	{
		return !entriesCache.empty();
	}

	const LogEntry& Logging::GetLastCachedEntry() const
	{
		if ( entriesCache.empty() )
			throw std::runtime_error( "Log cache is empty" );

		const auto& lastLog = entriesCache.back();

		return lastLog;
	}

	void Logging::ClearCache()
	{
		if ( !entriesCache.empty() )
			entriesCache.clear();
	}

	bool Logging::ErasePrevious( size_t count )
	{
		if ( entriesCache.empty() )
			return false;

		while ( !entriesCache.empty() && count-- != 0 )
		{
			entriesCache.pop_back();
		}

		return true;
	}
}
