﻿#pragma once

#include "../../Enum/kEnum.hpp"
#include "../../String/kStringConverter.hpp"
#include "../../Calendar/Time/kTimeComponentBase.hpp"

#include "../../../TypeTraits/TemplateTraits.hpp"

#include <atomic>
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
		using Pico = kCalendar::TimeComponentBase<std::chrono::duration<long long, std::femto>>;
		using Femto = kCalendar::TimeComponentBase<std::chrono::duration<long long, std::atto>>;

		ENUM_CLASS(UnitStrLength, uint8_t, SHORT, LONG);

		template<typename Units, class CharT = char, typename = std::enable_if_t<
			type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
			&& type_trait::Is_Char_V<CharT>
			>>
			USE_RESULT constexpr std::basic_string_view<CharT> GetUnitsStr(UnitStrLength length = UnitStrLength::SHORT) noexcept
		{
			if constexpr(std::is_same_v<Units, Hours>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"h", "hours"
				)
					);
			}
			else if constexpr(std::is_same_v<Units, Mins>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"m", "minutes"
				));
			}
			else if constexpr(std::is_same_v<Units, Secs>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"s", "seconds"
				));
			}
			else if constexpr(std::is_same_v<Units, Millis>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"ms", "milliseconds"
				));
			}
			else if constexpr(std::is_same_v<Units, Micros>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"us", "microseconds"
				));
			}
			else if constexpr(std::is_same_v<Units, Nanos>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"ns", "nanoseconds"
				));
			}
			else if constexpr(std::is_same_v<Units, Pico>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"ps", "picoseconds"
				));
			}
			else if constexpr(std::is_same_v<Units, Femto>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"fs", "femtoseconds"
				));
			}
			else
			{
				return kString::Convert<CharT>("");
			}
		}

		// template<typename Units, class CharT = char, typename = std::enable_if_t<
		// 	type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
		// 	&& type_trait::Is_Char_V<CharT>
		// 	>>
		// 	USE_RESULT constexpr std::basic_string_view<CharT> GetUnitsStr(Units&& value, UnitStrLength length) noexcept
		// {
		// 	return GetUnitsStr<Units, CharT>(length);
		// }
	}

	inline static auto SystemTickFrequency() noexcept
	{
		static const long long frequency = _Query_perf_frequency();	// doesn't change after system boot
		return frequency;
	}

	template<class Units = units::Micros, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
		>>
		struct HighAccuracyClock
	{   // Based on std::chrono::high_resolution_clock
		using Units_t = Units;
		using Rep_t = typename Units_t::Rep_t;
		using Period_t = typename Units_t::Period_t;
		using Duration_t = typename Units_t::Duration_t;
		using Underlying_t = std::chrono::high_resolution_clock;
		using TimePoint_t = std::chrono::time_point<Underlying_t, Duration_t>;
		static constexpr bool IsSteady = true;

		// Get current time
		USE_RESULT static constexpr TimePoint_t Now() noexcept
		{
			std::atomic_thread_fence(std::memory_order_relaxed);
			const auto frequency = SystemTickFrequency();
			const long long counter = _Query_perf_counter();
			//static_assert(Period_t::num == 1, "This assumes period::num == 1.");
			const long long whole = (counter / frequency) * Period_t::den;
			const long long part = (counter % frequency) * Period_t::den / frequency;
			const auto now = TimePoint_t(Duration_t(whole + part));
			std::atomic_thread_fence(std::memory_order_relaxed);
			return now;
		}
	};

	template<class Units = units::Micros, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
		>>
		struct SteadyClock
	{   // Based on std::chrono::steady_clock
		using Units_t = Units;
		using Rep_t = typename Units_t::Rep_t;
		using Period_t = typename Units_t::Period_t;
		using Duration_t = typename Units_t::Duration_t;
		using Underlying_t = std::chrono::steady_clock;
		using TimePoint_t = std::chrono::time_point<Underlying_t, Duration_t>;
		static constexpr bool IsSteady = HighAccuracyClock<Units_t>::IsSteady;

		USE_RESULT static constexpr decltype(auto) Now() noexcept
		{
			return HighAccuracyClock<Units_t>::Now();
		}
	};

	template<class Units = units::Micros, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
		>>
		struct SystemClock
	{   // Based on std::chrono::system_clock
		using Units_t = Units;
		using Rep_t = typename Units_t::Rep_t;
		using Period_t = std::chrono::ambiguous_local_time;
		using Duration_t = std::chrono::duration<Rep_t, Period_t>;
		using Underlying_t = std::chrono::system_clock;
		using TimePoint_t = std::chrono::time_point<Underlying_t, Duration_t>;
		static constexpr bool IsSteady = false;

		USE_RESULT static constexpr TimePoint_t Now() noexcept
		{
			std::atomic_thread_fence(std::memory_order_relaxed);
			const auto now = TimePoint_t(Duration_t(_Xtime_get_ticks()));
			std::atomic_thread_fence(std::memory_order_relaxed);
			return now;
		}

		USE_RESULT static decltype(auto) To_Time_t(const TimePoint_t& timePoint) noexcept
		{ // convert to __time64_t
			return Underlying_t::to_time_t(timePoint);
		}

		USE_RESULT static decltype(auto) From_Time_t(__time64_t time_t) noexcept
		{ // convert from __time64_t
			return Underlying_t::from_time_t(time_t);
		}
	};

	template<typename Rep, typename Clock>
	USE_RESULT constexpr Rep TimePointTo(const typename Clock::TimePoint_t& timePoint) noexcept(std::is_arithmetic_v<Rep>)
	{
		return static_cast<Rep>(
			std::chrono::time_point_cast<Clock::Duration_t>(timePoint).time_since_epoch().count()
			);
	}

	template<typename Rep, typename Units, typename Duration>
	USE_RESULT constexpr Rep DurationTo(const Duration& duration) noexcept(std::is_arithmetic_v<Rep>)
	{
		using Duration_t = typename Units::Duration_t;

		static constexpr auto sixtieth = (static_cast<double>(1) / 60);
		static constexpr auto thousandth = (static_cast<double>(1) / 1000);

		const auto diff = std::chrono::duration_cast<Duration_t>(duration);

		if constexpr(type_trait::Is_It_V<Units, units::Hours, units::Mins>)
			return static_cast<Rep>(sixtieth * diff.count());
		else
			return static_cast<Rep>(thousandth * diff.count());
	}
}
