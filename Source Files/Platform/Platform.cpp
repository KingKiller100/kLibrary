#include "pch.hpp"
#include "Platform.hpp"

namespace klib::kPlatform
{
	PlatformType GetPlatform() noexcept
	{
#if defined(_WIN64 ) || defined(_WIN32)
		return PlatformType::WINDOWS;
#elif defined(__APPLE__) || defined(APPLE)
		return PlatformType::APPLE;
#elif defined(__LINUX__) || defined(LINUX)
		return PlatformType::LINUX;
#endif
		return PlatformType::UNKNOWN;
	}
}
