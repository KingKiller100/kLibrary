#pragma once

namespace kTest::performance
{
	class PerformanceTestBase;
	
	void AddTest(PerformanceTestBase* test);

	void SetUpTests();
}
