#pragma once

namespace kTest
{
	template<class TesterMan>
	void AddTest(typename TesterMan::Test_t* test)
	{
		TesterMan::Get().Add(test);
	}
	
	namespace performance
	{
		void InitializeAlPerformanceTests();
	}

	void InitializeMathsTests();
	void InitializeUtilityTests();
	void InitializeTemplateTests();
	void InitializeAllTests();
}