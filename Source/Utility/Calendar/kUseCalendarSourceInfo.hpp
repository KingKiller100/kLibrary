#pragma once

#include "Secret/kiCalendarInfoSource.hpp"

namespace klib::kCalendar
{
	void UsePlatformCalendarInfoSource();
	secret::impl::iCalendarInfoSource& GetCalendarInfoSource();
	void SetCalendarInfoSource(secret::impl::iCalendarInfoSource* infoSource);	
}
