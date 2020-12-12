#pragma once

namespace kTest
{
	template<class TestManager>
	void AddTest(typename TestManager::Test_t* test)
	{
		TestManager::Get().Add(test);
	}
}
