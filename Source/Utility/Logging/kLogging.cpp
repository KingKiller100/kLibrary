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

	void Logging::Register( const LogProfile& profile, LogLevel level )
	{
		if ( logLevels.find( profile ) != logLevels.end() )
			return;

		logLevels.emplace( profile, level );
	}

	void Logging::SetLevel( const LogProfile& profile, LogLevel newMinLevel ) noexcept
	{
		logLevels.at( profile ) = newMinLevel;
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
		for ( auto& [_, level] : logLevels )
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

	void Logging::AddRaw( const std::string_view& text )
	{
		AddLog( LogEntry( LogProfile( "" ), text.data() ) );
	}

	void Logging::AddEntry( const LogProfile& profile, const LogLevel& level, const LogMessage& message )
	{
		if ( GetLevel( profile ) > level )
			return;

		AddLog( LogEntry(
			profile,
			message
		) );
	}

	void Logging::AddBanner(
		const std::string_view& descriptor
		, const LogMessage& message
		, const std::string_view& frontPadding
		, const std::string_view& backPadding
		, const std::uint16_t paddingCount
	)
	{
		std::string front, back;
		for ( auto i = 0; i < paddingCount; ++i )
		{
			front.append( frontPadding );
			back.append( backPadding );
		}

		const auto text = ToString<char>( tags::NoFormatTag{}
			, front
			, message.text
			, back
		);

		const LogMessage banner( text, message );

		AddLog( LogEntry( LogProfile( "" ), banner ) );
	}

	void Logging::AddLog( const LogEntry& entry )
	{
		entriesCache.push_back( entry );
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
			if ( !dest->Open() )
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
		return logLevels.at( profile );
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

		while ( count-- != 0 )
		{
			entriesCache.pop_back();
		}

		return true;
	}
}
