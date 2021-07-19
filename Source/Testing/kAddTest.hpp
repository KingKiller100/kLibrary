#pragma once

namespace kTest
{
	template<typename Test, typename TestManager>
	void AddTest(TestManager* tm)
	{
		tm->Add(new Test{});
	}
}
