#pragma once
#include "../../Source Files/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class ProfilerTester : public TesterBase
	{
	public:
		ProfilerTester();
		~ProfilerTester() override;

	protected:
		void Test() override;

	private:
		bool MicrosecondsTest();
		bool MillisecondsTest();
		bool SecondsTest();
	};
}
#endif
