#include "pch.hpp"
#include "kiCalendarInfoSource.hpp"

#include "../Platform/Windows/kCalenderInfoSourceWindows.hpp"
#include "../../Debug Helper/Exceptions/CalenderExceptions.hpp"
#include "../../../Platform/Platform.hpp"

#include <memory>



namespace klib::kCalendar::secret::helper
{
	iCalendarInfoSource& GetCalendarInfoSource()
	{
		static std::unique_ptr<iCalendarInfoSource> instance;

		if (!instance)
		{
			switch (kPlatform::GetPlatform())
			{
			case kPlatform::PlatformType::WINDOWS:
				instance.reset(new windows::CalendarInfoSourceWindows());
				break;
			case kPlatform::PlatformType::APPLE:
				//break;
			case kPlatform::PlatformType::LINUX:
				//break;
			default:
				throw kDebug::CalendarError("Unknown platform. Cannot create calendar information source");
				break;
			}
		}

		return *instance;
	}

}
