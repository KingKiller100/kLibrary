#pragma once

#include "../../Calendar/Time/kTimeComponentBase.hpp"
#include "../../../TypeTraits/TemplateTraits.hpp"
#include "../../Enum/kEnum.hpp"
#include "../../String/kStringConverter.hpp"

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
			&& type_trait::Is_CharType_V<CharT>
			>>
			USE_RESULT constexpr std::basic_string_view<CharT> GetUnitsStr(UnitStrLength length = UnitStrLength::SHORT) noexcept
		{
			if _CONSTEXPR_IF(std::is_same_v<Units, Hours>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"h", "hours"
				)
					);
			}
			else if _CONSTEXPR_IF(std::is_same_v<Units, Mins>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"m", "minutes"
				));
			}
			else if _CONSTEXPR_IF(std::is_same_v<Units, Secs>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"s", "seconds"
				));
			}
			else if _CONSTEXPR_IF(std::is_same_v<Units, Millis>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"ms", "milliseconds"
				));
			}
			else if _CONSTEXPR_IF(std::is_same_v<Units, Micros>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"us", "microseconds"
				));
			}
			else if _CONSTEXPR_IF(std::is_same_v<Units, Nanos>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"ns", "nanoseconds"
				));
			}
			else if _CONSTEXPR_IF(std::is_same_v<Units, Pico>)
			{
				return kString::Convert<CharT>(length.Compare(UnitStrLength::SHORT,
					"ps", "picoseconds"
				));
			}
			else if _CONSTEXPR_IF(std::is_same_v<Units, Femto>)
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

		template<typename Units, class CharT = char, typename = std::enable_if_t<
			type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
			&& type_trait::Is_CharType_V<CharT>
			>>
			USE_RESULT constexpr std::basic_string_view<CharT> GetUnitsStr(Units&& value, UnitStrLength length) noexcept
		{
			return GetUnitsStr<Units, CharT>(length);
		}


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
		using Blueprint_t = std::chrono::high_resolution_clock;
		using TimePoint_t = std::chrono::time_point<Blueprint_t, Duration_t>;

		USE_RESULT static constexpr decltype(auto) Now() noexcept
		{
			// get current time
			const long long _Freq = _Query_perf_frequency();	// doesn't change after system boot
			const long long _Ctr = _Query_perf_counter();
			static_assert(Period_t::num == 1, "This assumes period::num == 1.");
			const long long _Whole = (_Ctr / _Freq) * Period_t::den;
			const long long _Part = (_Ctr % _Freq) * Period_t::den / _Freq;
			return (TimePoint_t(Duration_t(_Whole + _Part)));
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
		using Blueprint_t = std::chrono::steady_clock;
		using TimePoint_t = std::chrono::time_point<Blueprint_t, Duration_t>;

		USE_RESULT static constexpr decltype(auto) Now() noexcept
		{
			return HighAccuracyClock<Units_t>::Now();
		}
	};

	template<class Units = units::Micros, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<Units, kCalendar::TimeComponentBase>
		>>
		struct SystemClock : private std::chrono::system_clock
	{   // Based on std::chrono::steady_clock
		using Units_t = Units;
		using Rep_t = typename Units_t::Rep_t;
		using Period_t = std::ratio_multiply<std::ratio<_XTIME_NSECS_PER_TICK, 1>, typename Units_t::Period_t>;
		using Duration_t = std::chrono::duration<Rep_t, Period_t>;
		using Blueprint_t = system_clock;
		using TimePoint_t = std::chrono::time_point<Blueprint_t, Duration_t>;

		USE_RESULT static constexpr decltype(auto) Now() noexcept
		{
			return TimePoint_t(Duration_t(_Xtime_get_ticks()));
		}

		USE_RESULT static decltype(auto) To_Time_t(const time_point& timePoint) noexcept
		{ // convert to __time64_t
			return to_time_t(timePoint);
		}

		USE_RESULT static decltype(auto) From_Time_t(__time64_t time_t) noexcept
		{ // convert from __time64_t
			return from_time_t(time_t);
		}
	};
}
