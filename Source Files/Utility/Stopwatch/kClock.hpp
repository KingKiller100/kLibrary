#pragma once

#include "../Calendar/Time/kTimeComponentBase.hpp"

#include <chrono>
#include <ratio>

namespace klib::kStopwatch
{
    namespace units
    {
        using Hours = kCalendar::TimeComponentBase<std::chrono::minutes>;
        using Mins = kCalendar::TimeComponentBase<std::chrono::seconds>;
        using Secs = kCalendar::TimeComponentBase<std::chrono::milliseconds>;
        using Millis = kCalendar::TimeComponentBase<std::chrono::microseconds>;
        using Micros = kCalendar::TimeComponentBase<std::chrono::nanoseconds>;
        using Nanos = kCalendar::TimeComponentBase<std::chrono::duration<long long, std::pico>>;
    }

	template<class Units = units::Micros, typename = std::enable_if_t<
        type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
        >>
    struct HighAccuracyClock : private std::chrono::high_resolution_clock
	{   // wraps std::chrono::high_resolution_clock
        using Units_t = Units;
    	using Rep_t = typename Units_t::Rep_t;
        using Period_t = typename Units_t::Period_t;
        using Duration_t = typename Units_t::Duration_t;
        using TimePoint_t = std::chrono::time_point<HighAccuracyClock, Duration_t>;
        using Underlying_t = std::chrono::high_resolution_clock;

    	USE_RESULT static constexpr decltype(auto) Now() noexcept
    	{
            return Underlying_t::now();
    	}
    };

	template<class Units = units::Micros, typename = std::enable_if_t<
        type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
        >>
    struct SteadyClock : private std::chrono::steady_clock
	{   // wraps std::chrono::high_resolution_clock
        using Units_t = Units;
    	using Rep_t = typename Units_t::Rep_t;
        using Period_t = typename Units_t::Period_t;
        using Duration_t = typename Units_t::Duration_t;
        using TimePoint_t = std::chrono::time_point<HighAccuracyClock, Duration_t>;
        using Underlying_t = std::chrono::steady_clock;

    	USE_RESULT static constexpr decltype(auto) Now() noexcept
    	{
            return Underlying_t::now();
    	}
    };

    template<class Units = units::Micros, typename = std::enable_if_t<
        type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
        >>
        struct SystemClock : private std::chrono::system_clock
    {   // wraps std::chrono::steady_clock
        using Units_t = Units;
        using Rep_t = typename Units_t::Rep_t;
        using Period_t = std::ratio_multiply<std::ratio<_XTIME_NSECS_PER_TICK, 1>, typename Units_t::Period_t>;
        using Duration_t = std::chrono::duration<Rep_t, Period_t>;
        using TimePoint_t = std::chrono::time_point<SystemClock, Duration_t>;
        using Underlying_t = std::chrono::system_clock;

        USE_RESULT static constexpr decltype(auto) Now() noexcept
        {
            return Underlying_t::now();
        }

        USE_RESULT static decltype(auto) To_Time_t(const time_point& timePoint) noexcept
    	{ // convert to __time64_t
            return Underlying_t::to_time_t(timePoint);
        }

        USE_RESULT static decltype(auto) From_Time_t(__time64_t time_t) noexcept
    	{ // convert from __time64_t
            return Underlying_t::from_time_t(time_t);
        }
    };
}
