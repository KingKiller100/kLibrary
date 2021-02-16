#include "pch.hpp"
#include "kPlatform.hpp"

namespace klib::kPlatform
{
	PlatformOS GetPlatform() noexcept
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
