#pragma once

#include "../../HelperMacros.hpp"

#include "../Enum/kEnum.hpp"

namespace klib
{
	namespace kPlatform
	{
		ENUM_CLASS(PlatformOS, std::uint16_t,
			UNKNOWN = 0,
			WINDOWS_32 = BIT_SHIFT(0),
			WINDOWS_64 = BIT_SHIFT(1),
			WINDOWS = WINDOWS_32 | WINDOWS_64,
			IOS_SIM = BIT_SHIFT(2),
			IOS = BIT_SHIFT(3),
			MAC = BIT_SHIFT(4),
			APPLE = IOS_SIM | IOS | MAC,
			ANDROID = BIT_SHIFT(5),
			LINUX = BIT_SHIFT(6) | ANDROID 
		);

		constexpr PlatformOS GetPlatform() noexcept
		{
#if defined(_WIN32) && !defined(_WIN64) 
			return PlatformOS::WINDOWS_32;

#elif defined(_WIN64)
			return PlatformOS::WINDOWS_64;

#elif defined(__APPLE__) || defined(__MACH__)

#	include <TargetConditionals.h>

#	if TARGET_IPHONE_SIMULATOR == 1
			return PlatformOS::IOS_SIM;
#	elif TARGET_OS_IPHONE == 1
			return PlatformOS::IOS;
#	elif TARGET_OS_MAC == 1
			return PlatformOS::MAC;
#	endif

#elif defined(__ANDROID__)
			return PlatformOS::ANDROID;

#elif defined(__LINUX__) || defined(__linux__) 
			return PlatformOS::LINUX;

#endif

			return PlatformOS::UNKNOWN;
		}
	}
 
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kPlatform;
#endif
	
}
