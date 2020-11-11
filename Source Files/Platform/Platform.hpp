#pragma once

#include "../HelperMacros.hpp"

#include "../Utility/EnumHelper/EnumHelper.hpp"

namespace klib::kPlatform
{
	ENUM_CLASS(PlatformType, std::uint8_t,
		UNKNOWN,
		WINDOWS,
		APPLE,
		LINUX
		);

	USE_RESULT PlatformType GetPlatform() noexcept;
}
