#pragma once

#include "../../../HelperMacros.hpp"

#include "../../String/Format/FormatSymbols.hpp"
#include "../../String/Format/Stringify/kStringifyInteger.hpp"

#include <string>
#include <unordered_map>


namespace klib::kLogs
{
	class LogEntry;
	class LogDescriptor;

	class iLoggerDestination
	{
	public:
		inline static constexpr auto DetailSpecifier = '&';

	protected:
		struct FormatSpecifier
		{
			char specifier;
			std::string index;
		};

		// inline static constexpr FormatSpecifier DaySpecifier = { 'd', '0' };
		// inline static constexpr FormatSpecifier MonthSpecifier = { 'm', '1' };
		// inline static constexpr FormatSpecifier YearSpecifier = { 'y', '2' };
		//
		// inline static constexpr FormatSpecifier HourSpecifier = { 'h', '3' };
		// inline static constexpr FormatSpecifier MinuteSpecifier = { 'z', '4' };
		// inline static constexpr FormatSpecifier SecondSpecifier = { 's', '5' };
		// inline static constexpr FormatSpecifier MillisecondSpecifier = { 'c', '6' };
		//
		// inline static constexpr FormatSpecifier NameSpecifier = { 'n', '7' };
		// inline static constexpr FormatSpecifier LogLevelSpecifier = { 'l', '8' };
		// inline static constexpr FormatSpecifier TextSpecifier = { 't', '9' };
		//
		// inline static constexpr FormatSpecifier SourceFileSpecifier = { 't', '9' };
		// inline static constexpr FormatSpecifier SourceLineSpecifier = { 't', '9' };
		// inline static constexpr FormatSpecifier SourceFuncSpecifier = { 't', '9' };

		inline static std::unordered_map<char, std::string> Specifiers = {
			{'d', "0"},
			{'m', "1"},
			{'y', "2"},
			{'h', "3"},
			{'z', "4"},
			{'s', "5"},
			{'c', "6"},
			{'n', "7"},
			{'l', "8"},
			{'t', "9"},
			{'q', "10"},
			{'w', "11"},
			{'e', "12"},
		};
		
	public:
		virtual ~iLoggerDestination() = default;

		/**
		 * \brief
		 *		Change name of the logger
		 * \param[in] newName
		 *		STL string representing a name
		 */
		virtual void SetName(std::string* newName) = 0;

		/**
		 * \brief
		 *		Sets the format of all log message
		 *		[Example] "[&dd/&mm/&yyyy] [&hh:&zz:&ss] [&n]: &t"
		 *		means "[01/01/1970] [01:12:59] [Logger]: Pass Test!
		 * \param format
		 *		Format of the log message for the destination logger
		 *		Declare each detail specifier item with a '&' character
		 *		Using multiple calls of the same specifiers gives different results
		 *		Detail specifiers:
		 *		- d/D = Day
		 *		- m/M = Month
		 *		- y/Y = Year
		 *		- h/H = Hours
		 *		- z/Z = Minutes
		 *		- s/S = Seconds
		 *		- c/C = Milliseconds
		 *		- n/N = Name
		 *		- t/T = Text
		 *		- q/Q = source file
		 *		- w/W = source line
		 *		- e/E = source function
		 */
		virtual void SetFormat(const std::string_view& format) noexcept = 0;

		/**
		 * \brief
		 *		Formats log message and level to the appropriate log message and then caches it
		 * \param[in] entry
		 *		Log entry containing message, time, date, log level, file and file line
		 */
		virtual void AddEntry(const LogEntry& entry) = 0;

		/**
		 * \brief
		 *		Returns if destination is open
		 * \return
		 *		TRUE if open, FALSE if not unopened
		 */
		virtual bool IsOpen() = 0;

		/**
		 * \brief
		 *		Opens up the log destination
		 */
		virtual bool Open() = 0;

		/**
		 * \brief
		 *		Closes the log destination
		 *	\param[in] outputClosingMsg
		 *			whether we should output the close message when closing
		 */
		virtual void Close(const bool outputClosingMsg) = 0;
	};

	class LogDestWithFormatSpecifier : public iLoggerDestination
	{
	public:
		void SetFormat(const std::string_view& format) noexcept override
		{
			logFormat.clear();
			
			const auto realFormat = kString::ToLower(format);

			for (size_t i = 0; i < realFormat.size(); ++i)
			{
				const auto& letter = realFormat[i];
				if (letter != DetailSpecifier)
					logFormat.push_back(letter);
				else
				{
					logFormat.push_back('{');

					const auto identifier = realFormat[i + 1];

					const auto& fi = Specifiers.at(identifier);

					logFormat.append(fi);

					const auto firstIndex = i + 1;

					auto lastIndex = realFormat.find_first_not_of(identifier, firstIndex);
					if (lastIndex == std::string::npos)
						lastIndex = firstIndex + 1;

					const auto count = lastIndex - firstIndex;

					if (count > 1)
					{
						logFormat.push_back(kString::format::g_SpecifierSymbol<char>);
						logFormat.append(kString::stringify::StringIntegral<char>(count));
					}

					logFormat.push_back('}');

					i += count;
				}
			}
		}

	protected:
		std::string logFormat;
	};
}

