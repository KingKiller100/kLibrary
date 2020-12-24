#pragma once

#include "Clock/kClock.hpp"
#include "kTimeSpan.hpp"

#include "../../HelperMacros.hpp"
#include "../../TypeTraits/TemplateTraits.hpp"

#include <atomic>


namespace klib
{
	namespace kStopwatch
	{
		template<typename Representation, class Clock = HighAccuracyClock<units::Micros>>
		class Stopwatch
		{
			using Clock_t = Clock;
			using Rep_t = Representation;
			using Units_t = typename Clock::Units_t;
			using TimePoint_t = typename Clock::TimePoint_t;

		public:
			constexpr Stopwatch() noexcept
				: start(Clock_t::Now())
				, previous(start)
				, elapsedTime(0)
				, isRunning(true)
			{ }

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetLifeTime() const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				const auto lifeTime = DurationTo<Rep_t, Units2>(Clock_t::Now() - start);
				return lifeTime;
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetElapsedTime() noexcept(std::is_arithmetic_v<Rep_t>)
			{
				if (isRunning)
				{
					RecordElapsedTime();
				}
				return DurationTo<Rep_t, Units2>(elapsedTime.GetDuration());
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetStartTime() const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				return DurationTo<Rep_t, Units2>(start.time_since_epoch());
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t Now() const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				return DurationTo<Rep_t, Units2>(Clock_t::Now().time_since_epoch());
			}

			void Stop() noexcept
			{
				isRunning = false;
				RecordElapsedTime();
			}

			void Restart() noexcept
			{
				isRunning = true;
				previous = Clock_t::Now();
			}

			bool IsRunning() const noexcept
			{
				return isRunning;
			}

			USE_RESULT constexpr TimeSpan GetTimeSpan() noexcept
			{
				return CreateTimeSpan(Clock_t::Now() - start);
			}

			USE_RESULT constexpr TimeSpan GetElapsedTimeSpan() noexcept
			{
				return CreateTimeSpan(elapsedTime.GetDuration());
			}

		protected:
			void RecordElapsedTime() noexcept
			{
				const auto now = Clock_t::Now();
				elapsedTime.GetDuration() = now - previous;
				previous = now;
			}
			
		private:
			const TimePoint_t start; // Time point at construction
			TimePoint_t previous; // Last record time point
			Units_t elapsedTime; // Elapsed time
			bool isRunning;
		};

		using HighAccuracyStopwatch = Stopwatch<double>;
		using SystemStopwatch = Stopwatch<double, SystemClock<units::Micros>>;
		using MonotonicStopwatch = Stopwatch<double, SteadyClock<units::Micros>>;

		using AccurateStopwatch = Stopwatch<float>;


		template<typename Representation, class Clock = HighAccuracyClock<units::Micros>>
		class AtomicStopwatch
		{
		public:
			using Clock_t = Clock;
			using Rep_t = Representation;
			using Units_t = typename Clock::Units_t;
			using TimePoint_t = typename Clock::TimePoint_t;

		public:
			constexpr AtomicStopwatch() noexcept
				: sw()
			{ }

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetLifeTime() const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				return sw.template GetLifeTime<Units2>();
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetDeltaTime() noexcept(std::is_arithmetic_v<Rep_t>)
			{
				std::atomic_thread_fence(std::memory_order_relaxed);
				const auto deltaTime = sw.template GetElapsedTime<Units2>();
				std::atomic_thread_fence(std::memory_order_relaxed);
				return deltaTime;
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t GetStartTime() noexcept(std::is_arithmetic_v<Rep_t>)
			{
				return sw.template GetStartTime<Units2>();
			}

			template<typename Units2 = Units_t, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<Units2, kCalendar::TimeComponentBase>
				>>
				USE_RESULT constexpr Rep_t Now() const noexcept(std::is_arithmetic_v<Rep_t>)
			{
				return sw.template Now<Units2>();
			}

			void Stop()
			{
				std::atomic_thread_fence(std::memory_order_relaxed);
				sw.Stop();
				std::atomic_thread_fence(std::memory_order_relaxed);
			}

			void Restart()
			{
				std::atomic_thread_fence(std::memory_order_relaxed);
				sw.Restart();
				std::atomic_thread_fence(std::memory_order_relaxed);
			}

			bool IsRunning() const
			{
				return sw.IsRunning();
			}

		private:
			Stopwatch<Representation, Clock> sw;
		};

		using AtomicHighAccStopwatch = AtomicStopwatch<double>;
		using AtomicSysStopwatch = AtomicStopwatch<double, SystemClock<units::Micros>>;
		using AtomicMonoStopwatch = AtomicStopwatch<double, SteadyClock<units::Micros>>;

		using AtomicAccStopwatch = AtomicStopwatch<float>;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kStopwatch;
#endif
}
