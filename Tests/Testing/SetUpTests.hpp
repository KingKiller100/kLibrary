#pragma once
#include "TesterManager.hpp"
#include "Performance/PerformanceTestManager.hpp"


namespace kTest
{
	namespace performance
	{
		void InitializeAllPerformanceTests(PerformanceTestManager& pm);
	}

	void InitializeMathsTests(TesterManager& tm);
	void InitializeUtilityTests(TesterManager& tm, bool includeTimeTests = true);
	void InitializeTemplateTests(TesterManager& tm);
	void InitializeAllTests(TesterManager& tm);
}
