#pragma once

#include "Secret/kiCalendarInfoSource.hpp"

namespace klib::kCalendar
{
	void UsePlatformCalendarInfoSource();
	void SetCalendarInfoSource( secret::impl::iCalendarInfoSource* infoSource );
}
