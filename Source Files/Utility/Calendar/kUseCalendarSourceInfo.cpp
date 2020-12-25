#include "pch.hpp"
#include "kUseCalendarSourceInfo.hpp"

#include "Platform/Windows/kCalenderInfoSourceWindows.hpp"
#include "../Debug/Exceptions/CalenderExceptions.hpp"
#include "../../Platform/kPlatform.hpp"

namespace klib::kCalendar
{
	std::unique_ptr<secret::impl::iCalendarInfoSource> g_kCalendarInfoSource;

	void UsePlatformCalendarInfoSource()
	{
		switch (kPlatform::GetPlatform())
		{
		case kPlatform::PlatformType::WINDOWS:
			SetCalendarInfoSource(new secret::impl::windows::CalendarInfoSourceWindows());
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

	secret::impl::iCalendarInfoSource& GetCalendarInfoSource()
	{
		return *g_kCalendarInfoSource;
	}

	void SetCalendarInfoSource(secret::impl::iCalendarInfoSource* infoSource)
	{
		g_kCalendarInfoSource.reset(infoSource);
	}
}
