#include "pch.hpp"
#include "kiLoggerDestination.hpp"
#include "../../String/Format/FormatSymbols.hpp"

namespace klib::kLogs
{
	const std::unordered_map<char, std::string> LogDestWithFormatSpecifier::LogFormatSpecifiersMap = {
		{'d', "0"}   // day
		, {'m', "1"} // month
		, {'y', "2"} // year
		, {'h', "3"} // hour
		, {'z', "4"} // minute
		, {'s', "5"} // second
		, {'c', "6"} // millisecond
		, {'n', "7"} // Profile name
		, {'p', "8"}  // Log descriptor [text]
		, {'w', "9"}  // Log descriptor [numeric]
		, {'t', "10"} // Log message
	};

	void LogDestWithFormatSpecifier::SetFormat( const std::string_view& format ) noexcept
	{
		messageFormat.clear();

		const auto realFormat = kString::ToLower( format );

		for ( size_t i = 0; i < realFormat.size(); ++i )
		{
			const auto& letter = realFormat[i];

			if ( letter != DetailSpecifier )
			{
				messageFormat.push_back( format[i] );
			}
			else
			{
				messageFormat.push_back( '{' );

				const auto identifier = realFormat[i + 1];

				const auto& fi = LogFormatSpecifiersMap.at( identifier );

				messageFormat.append( fi );

				const auto firstIndex = i + 1;

				auto lastIndex = realFormat.find_first_not_of( identifier, firstIndex );
				if ( lastIndex == std::string::npos )
					lastIndex = firstIndex + 1;

				const auto count = lastIndex - firstIndex;

				if ( count > 1 )
				{
					messageFormat.push_back( kString::format::g_ToStringSpecifierSymbol<char> );
					messageFormat.append( kString::stringify::StringIntegral<char>( count ) );
				}

				messageFormat.push_back( '}' );

				i += count;
			}
		}
	}
}
