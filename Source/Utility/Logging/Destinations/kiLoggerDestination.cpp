#include "pch.hpp"
#include "kiLoggerDestination.hpp"

namespace klib::kLogs
{
	const std::unordered_map<char, std::string> LogDestWithFormatSpecifier::LogFormatSpecifiersMap = {
			{'d', "0"},  // day
			{'m', "1"},  // month
			{'y', "2"},  // year
			{'h', "3"},  // hour
			{'z', "4"},  // minute
			{'s', "5"},  // second
			{'c', "6"},  // millisecond
			{'n', "7"},  // name
			{'p', "8"},  // Log descriptor [text]
			{'w', "9"},  // Log descriptor [numeric]
			{'t', "10"}, // Log message
			{'f', "11"}, // Source file
			{'l', "12"}, // Source line
			{'e', "13"}, // Source function
	};
}

