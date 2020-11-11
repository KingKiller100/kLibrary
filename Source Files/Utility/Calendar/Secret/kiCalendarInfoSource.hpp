#pragma once

#include "../../../HelperMacros.hpp"
#include <cstdint>

namespace klib::kCalendar
{
	enum class CalendarInfoSourceType;

	namespace secret::helper
	{
		// Singleton class
		class iCalendarInfoSource
		{
		public:
			virtual ~iCalendarInfoSource() = default;

			virtual void Refresh(CalendarInfoSourceType type) = 0;

			// Date info
			USE_RESULT virtual std::uint16_t GetDay() const = 0;
			USE_RESULT virtual std::uint16_t GetDayOfTheWeekIndex() const = 0;
			USE_RESULT virtual std::uint16_t GetMonth() const = 0;
			USE_RESULT virtual std::uint16_t GetYear() const = 0;

			// Time info
			USE_RESULT virtual std::uint16_t GetHour() const = 0;
			USE_RESULT virtual std::uint16_t GetMinute() const = 0;
			USE_RESULT virtual std::uint16_t GetSecond() const = 0;
			USE_RESULT virtual std::uint16_t GetMillisecond() const = 0;
		};

		iCalendarInfoSource& GetCalendarInfoSource();
	}
}
