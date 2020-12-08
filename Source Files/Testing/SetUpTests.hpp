#pragma once

namespace kTest
{
	template<class TestManager>
	void AddTest(typename TestManager::Test_t* test)
	{
		TestManager::Get().Add(test);
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