#pragma once

#include "../../HelperMacros.hpp"

#include "../Enum/kEnum.hpp"

namespace klib
{
	namespace kPlatform
	{
		ENUM_CLASS(PlatformOS, std::uint8_t,
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

		USE_RESULT PlatformOS GetPlatform() noexcept;
	}
 
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kPlatform;
#endif
	
}
