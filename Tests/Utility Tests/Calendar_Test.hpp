#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class CalendarTester final : public Tester
	{
	public:
		CalendarTester();
		~CalendarTester() override;
		
	private:
		void Test() override;

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

