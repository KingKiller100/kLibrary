#include "pch.hpp"
#include "kiCalendarInfoSource.hpp"

#include "../Platform/Windows/kCalenderInfoSourceWindows.hpp"
#include "../../Debug/Exceptions/CalenderExceptions.hpp"
#include "../../../Platform/Platform.hpp"

namespace klib::kCalendar::secret::impl
{
	std::unique_ptr<iCalendarInfoSource> g_kCalendarInfoSource;

	void UsePlatformCalendarInfoSource()
	{
		switch (kPlatform::GetPlatform())
		{
		case kPlatform::PlatformType::WINDOWS:
			SetCalendarInfoSource(new windows::CalendarInfoSourceWindows());
		case kPlatform::PlatformType::APPLE:
			//break;
		case kPlatform::PlatformType::LINUX:
			//break;
		default:
			throw kDebug::CalendarError("Unknown platform. Cannot create calendar information source");
			break;
		}
	}

	iCalendarInfoSource& GetCalendarInfoSource()
	{
		return *g_kCalendarInfoSource;
	}

	void SetCalendarInfoSource(iCalendarInfoSource* infoSource)
	{
		g_kCalendarInfoSource.reset(infoSource);
	}
}
