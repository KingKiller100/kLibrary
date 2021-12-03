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
	{}

	Logging::~Logging()
	{
		Close();
	}

	void Logging::Register( std::shared_ptr<LogProfile> profile )
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

	void Logging::SetGlobalLevel( const LogLevel newMin ) noexcept
	{
		for ( auto& profile : profiles )
		{
			profile->SetLevel( newMin );
		}
	}

	void Logging::AddRaw( std::string_view text )
	{
		AddLog( LogMessage( text ) );
	}

	void Logging::AddEntry( LogLevel level, const LogProfile& profile, const LogMessage& message )
	{
		if ( profile.GetLevel() > level )
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

		AddLog( banner );
	}

	void Logging::AddLog( const LogEntry& entry )
	{
		Flush( entry );
	}

	void Logging::AddLog( const LogMessage& message )
	{
		Flush( message );
	}

	void Logging::Flush( const LogEntry& entry )
	{
		for ( auto& dest : destinations )
		{
			dest->AddEntry( entry );
		}
	}

	void Logging::Flush( const LogMessage& message )
	{
		for ( auto& dest : destinations )
		{
			dest->AddRaw( message );
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
}
