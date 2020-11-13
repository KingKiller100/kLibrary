#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StopWatchTester : public Tester
	{
	public:
		StopWatchTester();
		~StopWatchTester() override;
		
	protected:
		void Test() override;
	};
}
#endif
