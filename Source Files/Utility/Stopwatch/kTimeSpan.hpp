#pragma once

#include <chrono>

#include "../../HelperMacros.hpp"
#include "Clock/kClock.hpp"

namespace klib
{
	namespace kStopwatch
	{
		struct TimeSpan
		{
		public:
			explicit TimeSpan(const std::chrono::hours h, const std::chrono::minutes m
				, const std::chrono::seconds s, const std::chrono::milliseconds ms) noexcept;

			std::chrono::hours hours;
			std::chrono::minutes minutes;
			std::chrono::seconds seconds;
			std::chrono::milliseconds milliseconds;
		};

		struct TimeSpanFull
		{
		public:
			explicit TimeSpanFull(const std::chrono::hours h, const std::chrono::minutes m
				, const std::chrono::seconds s, const std::chrono::milliseconds ms
				, const std::chrono::microseconds us, const std::chrono::nanoseconds ns) noexcept;

			std::chrono::hours hours;
			std::chrono::minutes minutes;
			std::chrono::seconds seconds;
			std::chrono::milliseconds milliseconds;
			std::chrono::microseconds microseconds;
			std::chrono::nanoseconds nanoseconds;
		};

		template<class Units = units::Micros, typename = std::enable_if_t<
			type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
			>>
			USE_RESULT constexpr auto CreateTimeSpan(const typename Units::Duration_t& duration) noexcept
		{
			using namespace std::chrono;
			using namespace units;

			if _CONSTEXPR17(type_trait::Is_It_V<Units, Pico, Femto>)
				return TimeSpan(hours(), minutes(), seconds(), milliseconds());
			else
			{
				constexpr auto milli_2_hour = 3'600'000;
				constexpr auto milli_2_min = 60'000;
				constexpr auto miili_2_sec = 1'000;

				auto asMillis = DurationTo<size_t, Millis>(duration);
				const hours hours(asMillis / milli_2_hour);
				asMillis %= milli_2_hour;
				const minutes mins(asMillis / milli_2_min);
				asMillis %= milli_2_min;
				const seconds secs(asMillis / miili_2_sec);
				asMillis %= miili_2_sec;
				const milliseconds millis(asMillis);

				return TimeSpan(hours, mins, secs, millis);
			}
		}

		template<class Units = units::Micros, typename = std::enable_if_t<
			type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
			>>
			USE_RESULT constexpr TimeSpanFull CreateTimeSpanFull(const typename Units::Duration_t& duration) noexcept
		{
			using namespace std::chrono;
			using namespace units;

			if _CONSTEXPR17(type_trait::Is_It_V<Units, Pico, Femto>)
				return TimeSpanFull(hours(), minutes(), seconds(), milliseconds()
					, microseconds(), nanoseconds());
			else
			{
				constexpr auto nano_2_hour = 3'600'000'000'000;
				constexpr auto nano_2_min = 60'000'000'000;
				constexpr auto nano_2_sec = 1'000'000'000;
				constexpr auto nano_2_milli = 1'000'000;
				constexpr auto nano_2_micro = 1'000;

				auto asNanos = DurationTo<size_t, Nanos>(duration);
				const hours hours(asNanos / nano_2_hour);
				asNanos %= nano_2_hour;
				const minutes mins(asNanos / nano_2_min);
				asNanos %= nano_2_min;
				const seconds secs(asNanos / nano_2_sec);
				asNanos %= nano_2_sec;
				const milliseconds millis(asNanos / nano_2_milli);
				asNanos %= nano_2_milli;
				const microseconds micros(asNanos / nano_2_micro);
				asNanos %= nano_2_micro;
				const nanoseconds nanos(asNanos);

				return TimeSpanFull(hours, mins, secs, millis, micros, nanos);
			}
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kStopwatch;
#endif
}
