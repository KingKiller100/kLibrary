#include "pch.hpp"
#include "kUseCalendarSourceInfo.hpp"

#include "Platform/Windows/kCalenderInfoSourceWindows.hpp"
#include "../Debug/Exceptions/CalenderExceptions.hpp"
#include "../../Platform/kPlatform.hpp"

namespace klib::kCalendar
{
	std::unique_ptr<secret::impl::iCalendarInfoSource> g_kCalendarInfoSource;

	using namespace secret::impl;
	
	void UsePlatformCalendarInfoSource()
	{
		switch (kPlatform::GetPlatform())
		{
		case kPlatform::PlatformType::WINDOWS:
			SetCalendarInfoSource(new windows::CalendarInfoSourceWindows());
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

	iCalendarInfoSource& GetCalendarInfoSource()
	{
		if (g_kCalendarInfoSource == nullptr)
		{
			throw kDebug::CalendarError("Calendar information source is not set." 
				" Please either set your own using \"SetCalendarInfoSource(iCalendarInfoSource*)\" or just call "
			" \"UsePlatformCalendarInfoSource(void)\"");
		}
		
		return *g_kCalendarInfoSource;
	}

	void SetCalendarInfoSource(iCalendarInfoSource* infoSource)
	{
		g_kCalendarInfoSource.reset(infoSource);
	}
}
