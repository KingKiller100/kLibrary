#pragma once

#include "../EnumHelper/EnumHelper.hpp"

#include <string_view>
#include <cstdint>

namespace klib
{
	namespace kLogs
	{		
		ENUM_CLASS(LogLevel, std::uint8_t,
			VBAT, // Verbatim
			DBUG, // Debug
			NORM, // Normal
			INFO, // Informative
			WARN, // Warning
			BANR, // Banner
			ERRR, // Error
			FATL  // Fatal
		);

	}
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif

}
