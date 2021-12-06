#include "pch.hpp"
#include "kiLoggerDestination.hpp"
#include "../../String/Format/FormatSymbols.hpp"
#include "../../String/Format/Stringify/kStringifyInteger.hpp"
#include "../../String/Tricks/kStringCases.hpp"

namespace klib::kLogs
{
	namespace
	{
		const std::unordered_map<char, std::string> g_LogFormatSpecifiersMap = {
			{'d', "0"}    // day
			, {'m', "1"}  // month
			, {'y', "2"}  // year
			, {'h', "3"}  // hour
			, {'z', "4"}  // minute
			, {'s', "5"}  // second
			, {'c', "6"}  // millisecond
			, {'n', "7"}  // Profile name
			, {'p', "8"}  // Log descriptor [text]
			, {'w', "9"}  // Log descriptor [numeric]
			, {'t', "10"} // Log message
		};
		const std::unordered_map<char, std::string> g_RawLogFormatSpecifiersMap = {
			{'d', "0"}   // day
			, {'m', "1"} // month
			, {'y', "2"} // year
			, {'h', "3"} // hour
			, {'z', "4"} // minute
			, {'s', "5"} // second
			, {'c', "6"} // millisecond
			, {'t', "7"} // Log message
		};
	}

	void FormattedLogDestinationBase::SetFormat( const std::string_view& format ) noexcept
	{
		SetFormatImpl( format, messageFormat, g_LogFormatSpecifiersMap );
	}

	void FormattedLogDestinationBase::SetRawFormat( const std::string_view& format ) noexcept
	{
		SetFormatImpl( format, rawMessageFormat, g_RawLogFormatSpecifiersMap );
	}

	void FormattedLogDestinationBase::SetFormatImpl(
		const std::string_view& format
		, std::string& outRealFormat
		, const std::unordered_map<char, std::string>& specifierMap
	) noexcept
	{
		outRealFormat.clear();

		const auto lowerCaseFmt = kString::ToLower( format );

		for ( size_t i = 0; i < lowerCaseFmt.size(); ++i )
		{
			const auto& letter = lowerCaseFmt[i];

			if ( letter != DetailSpecifier )
			{
				outRealFormat.push_back( format[i] );
			}
			else
			{
				outRealFormat.push_back( '{' );

				const auto identifier = lowerCaseFmt[i + 1];

				const auto& fi = specifierMap.at( identifier );

				outRealFormat.append( fi );

				const auto firstIndex = i + 1;

				auto lastIndex = lowerCaseFmt.find_first_not_of( identifier, firstIndex );
				if ( lastIndex == std::string::npos )
					lastIndex = firstIndex + 1;

				const auto count = lastIndex - firstIndex;

				if ( count > 1 )
				{
					outRealFormat.push_back( kString::format::g_ToStringSpecifierSymbol<char> );
					outRealFormat.append( kString::stringify::StringIntegral<char>( count ) );
				}

				outRealFormat.push_back( '}' );

				i += count;
			}
		}
	}
}
