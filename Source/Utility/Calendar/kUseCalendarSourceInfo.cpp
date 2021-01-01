#include "pch.hpp"
#include "kUseCalendarSourceInfo.hpp"

#include "Platform/Windows/kCalenderInfoSourceWindows.hpp"
#include "../Debug/Exceptions/CalenderExceptions.hpp"
#include "../../Platform/kPlatform.hpp"

namespace klib::kCalendar
{
	std::unique_ptr<secret::impl::iCalendarInfoSource> g_kCalendarInfoSource;

	using namespace secret::impl;
	using namespace kPlatform;
	
	void UsePlatformCalendarInfoSource()
	{
		auto os = GetPlatform();

		os = os.MaskCmp(PlatformOS::WINDOWS
			, PlatformOS::WINDOWS
			, os);
		
		os = os.MaskCmp(PlatformOS::APPLE
			, PlatformOS::APPLE
			, os);
		
		switch (os)
		{
		case PlatformOS::WINDOWS:
			SetCalendarInfoSource(new windows::CalendarInfoSourceWindows());
			break;
		case PlatformOS::APPLE:
			//break;
		case PlatformOS::LINUX:
			//break;
		default:
			throw kDebug::CalendarError("Unknown platform. Cannot create platform calendar information source");
			break;
		}
	}

	iCalendarInfoSource& GetCalendarInfoSource()
	{
		if (g_kCalendarInfoSource == nullptr)
		{
			throw kDebug::CalendarError("Calendar information source is not set." 
				" Please either set your own using \"SetCalendarInfoSource(iCalendarInfoSource*)\""
				" or just call \"UsePlatformCalendarInfoSource(void)\"");
		}
		
		return *g_kCalendarInfoSource;
	}

	void SetCalendarInfoSource(iCalendarInfoSource* infoSource)
	{
		g_kCalendarInfoSource.reset(infoSource);
	}
}
