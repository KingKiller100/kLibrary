#pragma once
#include "kDay.hpp"
#include "kMonth.hpp"
#include "kYear.hpp"

#include "../../../HelperMacros.hpp"

#include "../kCalenderInfoSourceType.hpp"

namespace klib
{
	namespace kCalendar
	{
		namespace secret::helper
		{
			class iCalendarInfoSource;
		}
		
		class Date : private CalendarComponentToStringImpl
		{
		public:
			enum DateTextLength : uint8_t
			{
				FULL,
				SHORT
			};

			enum DateNumericalSeparator : char
			{
				SLASH = '/',
				DASH = '-',
			};

		public:
			explicit Date(CalendarInfoSourceType sourceType);
			explicit Date(const Day::DayOfTheWeek dayOfTheWeek, const std::uint16_t d = 1, const Month::MonthOfTheYear m = kCalendar::Month::JAN, const std::uint16_t y = 1900);

			USE_RESULT std::string ToString(const std::string_view& format) const;
			USE_RESULT std::string ToString(DateNumericalSeparator separator = DateNumericalSeparator::SLASH) const;
			USE_RESULT std::string ToString(DateTextLength textLength) const;

			USE_RESULT const Day& GetDay() const;
			USE_RESULT Day GetDay();
			USE_RESULT const Month& GetMonth() const;
			USE_RESULT Month& GetMonth();
			USE_RESULT const Year& GetYear() const;
			USE_RESULT Year& GetYear();
			
		private:
			void CheckDate() const;
			explicit Date(const secret::helper::iCalendarInfoSource& source);

		private:
			Day day;
			Month month;
			Year year;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kCalendar;
#endif
}