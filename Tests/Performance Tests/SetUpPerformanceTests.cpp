#include "pch.hpp"
#include "SetUpPerformanceTests.hpp"

#include "PerformanceTestManager.hpp"

#include "Utility/ToString_SpeedTest.hpp"
#include "Maths/Vector_SpeedTest.hpp"
#include "Maths/Algorithms_SpeedTest.hpp"

namespace kTest::performance
{
	namespace
	{
		void AddUtilityTests()
		{
			// Utility
			AddTest(new util::ToStringSpeedTest());
		}

		void AddMathsTests()
		{
			// Maths
			AddTest(new maths::AlgorithmsSpeedTest());
			AddTest(new maths::VectorSpeedTest());
		}
	}

	void AddTest(PerformanceTestBase* test)
	{
		PerformanceTestManager::Get().Add(test);
	}

	void SetUpTests()
	{
		AddUtilityTests();
		AddMathsTests();
	}

}
