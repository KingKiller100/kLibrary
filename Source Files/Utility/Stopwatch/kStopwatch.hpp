#pragma once

#include "Clock/kClock.hpp"

#include "../../HelperMacros.hpp"
#include "../../TypeTraits/TemplateTraits.hpp"

#include <atomic>

namespace klib
{
	namespace kStopwatch
	{
		template<typename RepresentationType, class ClockType = HighAccuracyClock<units::Micros>>
		class Stopwatch
		{
			using Clock_t = ClockType;
			using Units_t = typename Clock_t::Units_t;
			using Rep_t = RepresentationType;
			using TimePoint_t = typename ClockType::TimePoint_t;

		public:
			constexpr Stopwatch() noexcept
				: startTimePoint(Clock_t::Now())
				, lastTimePoint(startTimePoint)
				, currentTimePoint(lastTimePoint)
				, isRunning(true)
			{ }

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetLifeTime() const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				std::atomic_thread_fence(std::memory_order_relaxed);
				const auto lifeTime = ConvertToUsableValue<Units2>(Clock_t::Now(), startTimePoint);
				std::atomic_thread_fence(std::memory_order_relaxed);
				return lifeTime;
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetDeltaTime() noexcept(std::is_arithmetic_v<Rep_t>)
			{
				std::atomic_thread_fence(std::memory_order_relaxed);

				if (IsRunning())
					currentTimePoint = Clock_t::Now();

				const auto deltaTime = ConvertToUsableValue<Units2>(currentTimePoint, lastTimePoint);
				lastTimePoint = currentTimePoint;

				std::atomic_thread_fence(std::memory_order_relaxed);

				return deltaTime;
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetStartTime() noexcept(std::is_arithmetic_v<Rep_t>)
			{
				using UnitsDuration_t = typename Units2::Duration_t;
				return static_cast<Rep_t>(
					std::chrono::time_point_cast<UnitsDuration_t>(startTimePoint).time_since_epoch().count()
					);
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t Now() const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				using UnitsDuration_t = typename Units2::Duration_t;
				return static_cast<Rep_t>(
					std::chrono::time_point_cast<UnitsDuration_t>(Clock_t::Now()).time_since_epoch().count()
					);
			}

			void Pause()
			{
				isRunning = false;
			}

			void Resume()
			{
				isRunning = true;
			}

			bool IsRunning() const
			{
				return isRunning;
			}

		protected:
			/**
			 * \brief
			 *		Converts stopwatch delta time recording to a type of time
			 *		i.e. converts stopwatch time to seconds
			 * \tparam Units2
			 *		Desired time units (if left blank, the initialized time unit type is used
			 * \param now
			 *		Time point now
			 * \param prev
			 *		Time point before
			 * \return
			 *		Time in the desired form, represented as your RepresentationType
			 */
			template<typename Units2>
			USE_RESULT constexpr Rep_t ConvertToUsableValue(const TimePoint_t& now
				, const TimePoint_t& prev) const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				using UnitsDuration_t = typename Units2::Duration_t;

				static constexpr double sixtieth = (CAST(long double, 1) / 60);
				static constexpr double thousandth = (CAST(long double, 1) / 1000);

				const auto diff = std::chrono::duration_cast<UnitsDuration_t>(now - prev);

				long double finalDuration;

				if _CONSTEXPR_IF(type_trait::Is_It_V<Units2, units::Hours, units::Mins>)
					finalDuration = sixtieth * diff.count();
				else
					finalDuration = thousandth * diff.count();

				return static_cast<Rep_t>(finalDuration);
			}

		private:
			const TimePoint_t startTimePoint;
			TimePoint_t lastTimePoint;
			TimePoint_t currentTimePoint;
			bool isRunning;
		};

		using HighAccuracyStopwatch = Stopwatch<double>;
		using SystemStopwatch = Stopwatch<double, SystemClock<units::Micros>>;
		using MonotonicStopwatch = Stopwatch<double, SteadyClock<units::Micros>>;

		using AccurateStopwatch = Stopwatch<float>;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kStopwatch;
#endif
}
