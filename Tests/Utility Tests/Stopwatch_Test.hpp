#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StopWatchTester : public TesterBase
	{
	public:
		StopWatchTester();
		~StopWatchTester() override;
		
	protected:
		void Prepare() override;
		
	private:
		bool GeneralTimeTest();
		bool PauseTest();
		bool MicrosecondsTest();
		bool MillisecondsTest();
		bool SecondsTest();
		bool MinutesTest();
		bool HoursTest();
	};
}
#endif
