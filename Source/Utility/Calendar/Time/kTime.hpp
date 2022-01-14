#pragma once

#include "../../../HelperMacros.hpp"

#include "../kCalenderInfoSourceType.hpp"

#include <cstdint>
#include <string>


#include "kHour.hpp"
#include "kMinute.hpp"
#include "kSecond.hpp"
#include "kMilliseconds.hpp"

namespace klib
{
	namespace kCalendar
	{
		class Time : private CalendarComponentToStringImpl
		{
		public:
			enum TimeComponent : uint8_t
			{
				HOURS,
				MINS,
				SECS,
				MILLIS,
				END_MARKER
			};
			
		public:
			explicit Time(CalendarInfoSourceType sourceType);
			explicit Time(
				std::uint16_t h, std::uint16_t m, std::uint16_t s, std::uint16_t ms = 0, Hour::CycleType cycle = Hour::CYCLE_24);

			USE_RESULT std::uint16_t GetComponent( TimeComponent timeComponent) const;
			USE_RESULT std::string ToString( TimeComponent accuracy = MILLIS) const;
			USE_RESULT std::string ToString(const std::string_view& format) const;

			USE_RESULT Hour& GetHour();
			USE_RESULT const Hour& GetHour() const;
			USE_RESULT Minute& GetMinute();
			USE_RESULT const Minute& GetMinute() const;
			USE_RESULT Second& GetSecond();
			USE_RESULT const Second& GetSecond() const;
			USE_RESULT Millisecond& GetMillisecond();
			USE_RESULT const Millisecond& GetMillisecond() const;
			
		private:
			void CheckTime() const;
			
		private:
			Hour hour;
			Minute minute;
			Second second;
			Millisecond millisecond;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
		using namespace kCalendar;
#endif
}
