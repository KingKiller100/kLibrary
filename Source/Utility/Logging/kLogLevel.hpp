﻿#pragma once

#include "../Enum/kEnum.hpp"

#include <cstdint>

namespace klib
{
	namespace kLogs
	{
		ENUM_CLASS(LogLevel, std::uint8_t,
			RAW, // Verbatim
			TRC, // Trace
			DBG, // Debug
			NRM, // Normal
			INF, // Informative
			WRN, // Warning
			BNR, // Banner
			ERR, // Error
			FTL  // Fatal
		);
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kLogs;
#endif

}
