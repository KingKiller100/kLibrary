#pragma once

namespace kTest
{
	namespace performance
	{
		void InitializeAllPerformanceTests();
	}

	void InitializeMathsTests();
	void InitializeUtilityTests(bool includeTimeTests);
	void InitializeTemplateTests();
	void InitializeAllTests();
}
