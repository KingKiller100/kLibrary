#include "pch.hpp"
#include "../Utility Tests/Stopwatch_Test.hpp"

#include "../../Source Files/Maths/kAlgorithms.hpp"
#include "../../Source Files/Utility/Stopwatch/kStopwatch.hpp"
#include "../../Source Files/Utility/String/kToString.hpp"

#include <thread>


#ifdef TESTING_ENABLED
namespace kTest::utility
{
	StopWatchTester::StopWatchTester()
		: Tester("Stopwatch Test")
	{	}

	StopWatchTester::~StopWatchTester()
		= default;

	void StopWatchTester::Test()
	{
		std::string outputStr;

		const auto maxCount = 99999;
		int nums[maxCount];

		const auto processorCount = std::thread::hardware_concurrency();
		const size_t size = 5000 * processorCount;

		const auto loops = kmaths::Min(size, maxCount);

		klib::kStopwatch::HighAccuracyStopwatch testTime("Stopwatch Timer");

		for (auto i = 0; i < loops; i++)
		{
			const auto dt = testTime.GetDeltaTime<klib::kStopwatch::units::Micros>();
			nums[i] = i;
			VERIFY(nums[i] == i && dt != 0);
			outputStr = klib::kString::stringify::SprintfWrapper("Test Time %d : %fus (Microseconds)\n", i, dt);
		}

		const auto lifetime = testTime.GetLifeTime<klib::kStopwatch::units::Micros>();
		const auto lifetimeInSeconds = lifetime / 1000000;
	}
}
#endif
