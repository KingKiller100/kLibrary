#include "pch.hpp"
#include "../Utility Tests/Stopwatch_Test.hpp"

#include "../../Source Files/Maths/kMathsApproximately.hpp"
#include "../../Source Files/Utility/Stopwatch/kStopwatch.hpp"
#include "../../Source Files/Utility/String/kToString.hpp"

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

	void StopWatchTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(GeneralTimeTest());
		VERIFY_MULTI(PauseTest());
		VERIFY_MULTI(MicrosecondsTest());
		VERIFY_MULTI(MillisecondsTest());
		VERIFY_MULTI(SecondsTest());
		// VERIFY_MULTI(MinutesTest());
		// VERIFY_MULTI(HoursTest()); // Too long to test
		VERIFY_MULTI_END();
	}

	bool StopWatchTester::GeneralTimeTest()
	{
		{
			constexpr auto allowance = 50;
			
			Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;
			sw.Restart();
			std::this_thread::sleep_for(750ms);
			const auto dt1 = sw.GetDeltaTime();
			std::this_thread::sleep_for(750ms);
			const auto dt2 = sw.GetDeltaTime();
			std::this_thread::sleep_for(750ms);
			const auto dt3 = sw.GetDeltaTime();
			const auto lifeTime = sw.GetLifeTime();

			VERIFY(kmaths::Approximately(lifeTime, 750, allowance));
			VERIFY(kmaths::Approximately(dt1, 750, allowance));
			VERIFY(kmaths::Approximately(dt2, 750, allowance));
			VERIFY(kmaths::Approximately(dt3, 750, allowance));
		}

		{
			Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;
			std::this_thread::sleep_for(10ms);
			const auto dt1 = sw.GetDeltaTime();
			std::this_thread::sleep_for(5ms);
			const auto dt2 = sw.GetDeltaTime();
			std::this_thread::sleep_for(20ms);
			const auto dt3 = sw.GetDeltaTime();
			const auto lifeTime = sw.GetLifeTime();

			VERIFY(kmaths::Approximately(lifeTime, 35, 5));
			VERIFY(kmaths::Approximately(dt1, 10, 5));
			VERIFY(kmaths::Approximately(dt2, 5, 2));
			VERIFY(kmaths::Approximately(dt3, 20, 5));
		}

		return success;
	}

	bool StopWatchTester::PauseTest()
	{
		Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;

		std::this_thread::sleep_for(25ms);

		sw.Stop();
		const auto dt = sw.GetDeltaTime();
		const auto lt = sw.GetLifeTime();
		VERIFY(!sw.IsRunning());
		VERIFY(kmaths::Approximately(dt, 25, 1));
		VERIFY(kmaths::Approximately(lt, 25, 1));

		auto now = sw.GetDeltaTime();
		VERIFY(dt == now);
		VERIFY(lt == now);

		std::this_thread::sleep_for(5ms);

		sw.Restart();

		std::this_thread::sleep_for(5ms);

		now = sw.GetDeltaTime();
		VERIFY(sw.IsRunning());
		VERIFY(dt < now);

		return success;
	}

	bool StopWatchTester::MicrosecondsTest()
	{
		constexpr auto duration = 100'000us;
		constexpr auto allowance = 1050;
		Stopwatch<std::time_t, HighAccuracyClock<units::Micros>> sw;
		std::this_thread::sleep_for(duration);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		return success;
	}

	bool StopWatchTester::MillisecondsTest()
	{
		constexpr auto duration = 100ms;
		constexpr auto allowance = 5;
		Stopwatch<float, HighAccuracyClock<units::Millis>> sw;
		std::this_thread::sleep_for(duration);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		return success;
	}

	bool StopWatchTester::SecondsTest()
	{
		constexpr auto duration = 0.5s;
		constexpr auto allowance = 0;
		Stopwatch<std::time_t, HighAccuracyClock<units::Secs>> sw;
		std::this_thread::sleep_for(duration);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		return success;
	}

	bool StopWatchTester::MinutesTest()
	{
		constexpr auto duration = 0.25min;
		constexpr auto allowance = 0;
		Stopwatch<double, HighAccuracyClock<units::Mins>> sw;
		std::this_thread::sleep_for(duration);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		return success;
	}

	bool StopWatchTester::HoursTest()
	{
		constexpr auto duration = 0.25h;
		constexpr auto allowance = 0;
		Stopwatch<double, HighAccuracyClock<units::Hours>> sw;
		std::this_thread::sleep_for(duration);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, duration.count(), allowance));

		return success;
	}
}
#endif
