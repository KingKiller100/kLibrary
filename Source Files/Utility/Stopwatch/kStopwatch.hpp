#pragma once

#include "../../HelperMacros.hpp"

#include "../Calendar/Time/kTimeComponentBase.hpp"

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
	}

	template<typename RepresentationType = double, typename ClockType = std::chrono::high_resolution_clock>
	class Stopwatch
	{
		using RepT = RepresentationType;
		using ClockT = ClockType;
		using TimePointT = typename ClockType::time_point;

	public:

		constexpr Stopwatch(const char* name) noexcept
			: name(name)
			, startTimePoint(ClockT::now())
			, lastTimePoint(startTimePoint)
		{ }

		USE_RESULT constexpr const char* GetName() const noexcept
		{
			return name;
		}

		template<typename Units, typename = std::enable_if_t<
			std::_Is_specialization_v<Units, kCalendar::TimeComponentBase>
			>>
			USE_RESULT constexpr RepT GetLifeTime() const noexcept(std::is_arithmetic_v<RepT>)
		{
			std::atomic_thread_fence(std::memory_order_relaxed);
			const auto lifeTime = ConvertToUsableValue<Units>(ClockT::now(), startTimePoint);
			std::atomic_thread_fence(std::memory_order_relaxed);
			return lifeTime;
		}

		template<typename Units, typename = std::enable_if_t<
			std::_Is_specialization_v<Units, kCalendar::TimeComponentBase>
			>>
			USE_RESULT constexpr RepT GetDeltaTime() noexcept(std::is_arithmetic_v<RepT>)
		{
			std::atomic_thread_fence(std::memory_order_relaxed);

			const auto currentTimePoint = ClockT::now();
			const auto deltaTime = ConvertToUsableValue<Units>(currentTimePoint, lastTimePoint);
			lastTimePoint = currentTimePoint;

			std::atomic_thread_fence(std::memory_order_relaxed);

			return deltaTime;
		}

		template<typename Units, typename = std::enable_if_t<
			std::_Is_specialization_v<Units, kCalendar::TimeComponentBase>
			>>
			USE_RESULT constexpr RepT GetStartTime() noexcept(std::is_arithmetic_v<RepT>)
		{
			using UnitsUnderlyingT = typename Units::UnderlyingT;
			return std::chrono::time_point_cast<UnitsUnderlyingT>(startTimePoint).time_since_epoch().count();
		}

		template<typename Units, typename = std::enable_if_t<
			std::_Is_specialization_v<Units, kCalendar::TimeComponentBase>
			>>
			USE_RESULT constexpr RepT Now() const noexcept(std::is_arithmetic_v<RepT>)
		{
			using UnitsUnderlyingT = typename Units::UnderlyingT;
			const auto currentTimePoint = ClockT::now();
			return std::chrono::time_point_cast<UnitsUnderlyingT>(currentTimePoint).time_since_epoch().count();
		}

	protected:
		template<typename Units, typename = std::enable_if_t<
			std::_Is_specialization_v<Units, kCalendar::TimeComponentBase>
			>>
			USE_RESULT constexpr RepT ConvertToUsableValue(const TimePointT& now
				, const TimePointT& prev) const noexcept(std::is_arithmetic_v<RepT>)
		{
			using UnitsUnderlyingT = typename Units::UnderlyingT;

			static constexpr double sixtieth = (CAST(long double, 1) / 60);
			static constexpr double thousandth = (CAST(long double, 1) / 1000);

			const auto duration = std::chrono::duration_cast<UnitsUnderlyingT>(now - prev);

			long double finalDuration;

			if _CONSTEXPR_IF(std::is_same_v<Units, units::Hours>
				|| std::is_same_v<Units, units::Mins>)
				finalDuration = duration.count() * sixtieth;
			else
				finalDuration = duration.count() * thousandth;

			return static_cast<RepT>(finalDuration);
		}

	private:
		const char* name;

		const TimePointT startTimePoint;
		TimePointT lastTimePoint;
	};

	using HighAccuracyStopwatch = Stopwatch<>;
	using SystemStopwatch = Stopwatch<double, std::chrono::system_clock>;
	using MonotonicStopwatch = Stopwatch<double, std::chrono::steady_clock>;

	using HighAccuracyTimerf = Stopwatch<float>;
	using SystemTimerf = Stopwatch<float, std::chrono::system_clock>;
	using MonotonicTimerf = Stopwatch<float, std::chrono::steady_clock>;
}
