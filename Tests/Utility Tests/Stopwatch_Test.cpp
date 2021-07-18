#include "pch.hpp"
#include "../Utility Tests/Stopwatch_Test.hpp"

#include "../../Source/Maths/kMathsApproximately.hpp"
#include "../../Source/Utility/Stopwatch/kStopwatch.hpp"
#include "../../Source/Utility/String/kToString.hpp"

#include <thread>

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	StopWatchTester::StopWatchTester()
		: TesterBase("Stopwatch Test")
	{	}

	StopWatchTester::~StopWatchTester()
		= default;

	using namespace klib::kStopwatch;
	using namespace std::chrono_literals;

	void StopWatchTester::Prepare()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(GeneralTimeTest());
		VERIFY_MULTI(PauseTest());
		// VERIFY_MULTI(MicrosecondsTest());
		// VERIFY_MULTI(MillisecondsTest());
		VERIFY_MULTI(SecondsTest());
		// VERIFY_MULTI(MinutesTest());
		// VERIFY_MULTI(HoursTest()); // Too long to test
		VERIFY_MULTI_END();
	}

	bool StopWatchTester::GeneralTimeTest()
	{
		{
			constexpr auto pauseTime = 750ms;
			constexpr std::time_t allowance = 250;

			constexpr auto expectedElapsedTime = pauseTime.count() + allowance - 1;
			constexpr auto expectedLifeTime = (pauseTime * 3).count() + allowance - 1;
			
			Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;
			sw.Restart();
			std::this_thread::sleep_for(pauseTime);
			sw.Stop();
			const auto dt1 = sw.GetElapsedTime();
			sw.Restart();
			std::this_thread::sleep_for(pauseTime);
			sw.Stop();
			const auto dt2 = sw.GetElapsedTime();
			sw.Restart();
			std::this_thread::sleep_for(pauseTime);
			sw.Stop();
			const auto dt3 = sw.GetElapsedTime();
			const auto lifeTime = sw.GetLifeTime();
			
			VERIFY(kmaths::Approximately(lifeTime, expectedLifeTime, allowance));
			VERIFY(kmaths::Approximately(dt1, expectedElapsedTime, allowance));
			VERIFY(kmaths::Approximately(dt2, expectedElapsedTime, allowance));
			VERIFY(kmaths::Approximately(dt3, expectedElapsedTime, allowance));
		}

		{
			constexpr auto pauseTime1 = 100ms;
			constexpr auto pauseTime2 = 200ms;
			constexpr std::time_t allowance = 250;

			constexpr auto expectedElapsedTime1 = pauseTime1.count() + allowance - 1;
			constexpr auto expectedElapsedTime2 = pauseTime2.count() + allowance - 1;
			constexpr auto expectedLifeTime = (pauseTime1 * 2 + pauseTime2).count() + allowance - 1;

			Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;
			sw.Restart();
			std::this_thread::sleep_for(100ms);
			const auto dt1 = sw.GetElapsedTime();
			std::this_thread::sleep_for(100ms);
			const auto dt2 = sw.GetElapsedTime();
			std::this_thread::sleep_for(200ms);
			sw.Stop();
			const auto dt3 = sw.GetElapsedTime();
			const auto lifeTime = sw.GetLifeTime();

			VERIFY(kmaths::Approximately(lifeTime, expectedLifeTime, allowance));
			VERIFY(kmaths::Approximately(dt1, expectedElapsedTime1, allowance));
			VERIFY(kmaths::Approximately(dt2, expectedElapsedTime1, allowance));
			VERIFY(kmaths::Approximately(dt3, expectedElapsedTime2, allowance));
		}

		
	}

	bool StopWatchTester::PauseTest()
	{
		constexpr auto allowance = 50;
		
		Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;

		std::this_thread::sleep_for(100ms);

		sw.Stop();
		const auto dt = sw.GetElapsedTime();
		const auto lt = sw.GetLifeTime();
		VERIFY(!sw.IsRunning());
		VERIFY(kmaths::Approximately(dt, 100, allowance));
		VERIFY(kmaths::Approximately(lt, 100, allowance));

		auto elapsed = sw.GetElapsedTime();
		VERIFY(dt == elapsed);
		VERIFY(lt == elapsed);

		std::this_thread::sleep_for(75ms);

		elapsed = sw.GetElapsedTime();
		VERIFY(dt == elapsed);
		VERIFY(lt == elapsed);
		
		sw.Restart();

		std::this_thread::sleep_for(200ms);

		VERIFY(sw.IsRunning());
		
		sw.Stop();
		elapsed = sw.GetElapsedTime();
		VERIFY(!sw.IsRunning());
		VERIFY(dt < elapsed);

		
	}

	bool StopWatchTester::MicrosecondsTest()
	{
		constexpr auto duration = 100'000us;
		constexpr auto allowance = 5000;
		Stopwatch<std::time_t, HighAccuracyClock<units::Micros>> sw;
		sw.Restart();
		std::this_thread::sleep_for(duration);
		sw.Stop();
		const auto now = sw.GetElapsedTime();
		VERIFY(kmaths::Approximately(now, duration.count() + allowance, allowance));

		
	}

	bool StopWatchTester::MillisecondsTest()
	{
		constexpr auto duration = 100ms;
		constexpr auto allowance = 25;
		Stopwatch<float, HighAccuracyClock<units::Micros>> sw;
		sw.Restart();
		std::this_thread::sleep_for(duration);
		sw.Stop();
		const auto now = sw.GetElapsedTime<units::Millis>();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		
	}

	bool StopWatchTester::SecondsTest()
	{
		constexpr auto duration = 0.5s;
		constexpr auto allowance = 0.05f;
		Stopwatch<float, HighAccuracyClock<units::Secs>> sw;
		sw.Restart();
		std::this_thread::sleep_for(duration);
		sw.Stop();
		const auto now = sw.GetElapsedTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		
	}

	bool StopWatchTester::MinutesTest()
	{
		constexpr auto duration = 0.25min;
		constexpr auto allowance = 0;
		Stopwatch<double, HighAccuracyClock<units::Mins>> sw;
		sw.Restart();
		sw.Stop();
		std::this_thread::sleep_for(duration);
		const auto now = sw.GetElapsedTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		
	}

	bool StopWatchTester::HoursTest()
	{
		constexpr auto duration = 0.25h;
		constexpr auto allowance = 0;
		Stopwatch<double, HighAccuracyClock<units::Hours>> sw;
		sw.Restart();
		std::this_thread::sleep_for(duration);
		sw.Stop();
		const auto now = sw.GetElapsedTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		
	}
}
#endif
