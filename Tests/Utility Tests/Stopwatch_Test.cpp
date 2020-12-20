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
		// VERIFY_MULTI(GeneralTimeTest());
		// VERIFY_MULTI(NanosecondsTest());
		// VERIFY_MULTI(MicrosecondsTest());
		// VERIFY_MULTI(MillisecondsTest());
		// VERIFY_MULTI(SecondsTest());
		// VERIFY_MULTI(MinutesTest());
		// VERIFY_MULTI(HoursTest());
		VERIFY_MULTI_END();
	}

	bool StopWatchTester::GeneralTimeTest()
	{
		{
			Stopwatch<std::time_t> sw;
			std::this_thread::sleep_for(10ms);
			auto dt = sw.GetDeltaTime();
			VERIFY(kmaths::Approximately(dt, 10, 1));
			std::this_thread::sleep_for(10ms);
			dt = sw.GetDeltaTime();
			VERIFY(kmaths::Approximately(dt, 10, 1));
			std::this_thread::sleep_for(10ms);
			dt = sw.GetDeltaTime();
			VERIFY(kmaths::Approximately(dt, 10, 1));
			const auto lifeTime = sw.GetLifeTime();
			VERIFY(kmaths::Approximately(lifeTime, 30, 1));
		}

		return success;
	}

	bool StopWatchTester::PauseTest()
	{
		Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;
		std::this_thread::sleep_for(25ms);
		const auto dt = sw.GetDeltaTime();
		VERIFY(sw.IsRunning());
		VERIFY(kmaths::Approximately(dt, 25, 1));
		sw.Pause();
		VERIFY(!sw.IsRunning());
		auto now = sw.GetDeltaTime();
		VERIFY(dt == now);
		sw.Resume();
		VERIFY(sw.IsRunning());
		now = sw.GetDeltaTime();
		VERIFY(dt < now);

		return success;
	}

	bool StopWatchTester::NanosecondsTest()
	{
		Stopwatch<std::time_t, HighAccuracyClock<units::Nanos>> sw;
		std::this_thread::sleep_for(500ns);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, 500, 1));

		return success;
	}

	bool StopWatchTester::MicrosecondsTest()
	{
		Stopwatch<std::time_t, HighAccuracyClock<units::Micros>> sw;
		std::this_thread::sleep_for(500us);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, 500, 1));

		return success;
	}

	bool StopWatchTester::MillisecondsTest()
	{
		Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw;
		std::this_thread::sleep_for(500ms);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, 500, 1));

		return success;
	}

	bool StopWatchTester::SecondsTest()
	{
		Stopwatch<double, HighAccuracyClock<units::Secs>> sw;
		std::this_thread::sleep_for(1s);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, 1, .25));

		return success;
	}

	bool StopWatchTester::MinutesTest()
	{
		Stopwatch<float, HighAccuracyClock<units::Mins>> sw;
		std::this_thread::sleep_for(1min);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, 1.0f, 0.5f));

		return success;
	}

	bool StopWatchTester::HoursTest()
	{
		Stopwatch<float, HighAccuracyClock<units::Mins>> sw;
		
		std::this_thread::sleep_for(30min);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, 0.5f, 0.1f));

		return success;
	}
}
#endif
