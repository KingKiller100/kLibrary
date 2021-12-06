#pragma once
#include "../IncludeTesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StopWatchTester : public TesterBase
	{
	public:
		StopWatchTester();
		~StopWatchTester() override;
		
	protected:
		void Prepare() noexcept override;
		
	private:
		void GeneralTimeTest();
		void PauseTest();
		void MicrosecondsTest();
		void MillisecondsTest();
		void SecondsTest();
		void MinutesTest();
		void HoursTest();
	};
}
#endif
