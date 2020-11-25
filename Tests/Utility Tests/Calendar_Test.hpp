#pragma once
#include "../../Source Files/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class CalendarTester final : public TesterBase
	{
	public:
		CalendarTester();
		~CalendarTester() override;
		
	protected:
		void Test() override;

	private:
		bool DayTest();
		bool MonthTest();
		bool YearTest();
		bool MillisecondTest();
		bool SecondTest();
		bool MinuteTest();
		bool HourTest();
		
		bool GetTimeTextTest();
		bool DayOfTheWeekTest();

		bool ToStringTest();
		
		bool GetTimeComponentTest();
		bool GetDateInTextFormatTest();
		bool GetDateInNumericalFormatTest();

		bool CreateTimeTest();
	};
}
#endif

