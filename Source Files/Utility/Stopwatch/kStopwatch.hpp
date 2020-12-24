#pragma once

#include "Clock/kClock.hpp"
#include "kTimeSpan.hpp"

#include "../../HelperMacros.hpp"
#include "../../TypeTraits/TemplateTraits.hpp"

#include <atomic>
#include <mutex>


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
				return CreateTimeSpan(elapsedTime);
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
		class ThreadStopwatch
		{
		public:
			using Clock_t = Clock;
			using Rep_t = Representation;
			using Units_t = typename Clock::Units_t;
			using TimePoint_t = typename Clock::TimePoint_t;

		public:
			constexpr ThreadStopwatch() noexcept
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
				const auto scopeLock = std::scoped_lock(mutex);
				return sw.template GetElapsedTime<Units2>();
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
				const auto scopeLock = std::scoped_lock(mutex);
				sw.Stop();
			}

			void Restart()
			{
				const auto scopeLock = std::scoped_lock(mutex);
				sw.Restart();
			}

			bool IsRunning() const
			{
				return sw.IsRunning();
			}

			USE_RESULT constexpr TimeSpan GetTimeSpan() noexcept
			{
				return sw.GetTimeSpan();
			}

			USE_RESULT constexpr TimeSpan GetElapsedTimeSpan() noexcept
			{
				return sw.GetElapsedTimeSpan();
			}

		private:
			Stopwatch<Representation, Clock> sw;
			std::mutex mutex;
		};

		using ThreadHighAccStopwatch = ThreadStopwatch<double>;
		using ThreadSysStopwatch = ThreadStopwatch<double, SystemClock<units::Micros>>;
		using ThreadMonoStopwatch = ThreadStopwatch<double, SteadyClock<units::Micros>>;

		using ThreadAccStopwatch = ThreadStopwatch<float>;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kStopwatch;
#endif
}
