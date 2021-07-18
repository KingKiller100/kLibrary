#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class CalendarTester final : public TesterBase
	{
	public:
		CalendarTester();
		~CalendarTester() override;
		
	protected:
		void Prepare() noexcept override;

	private:
		void DayTest();
		void MonthTest();
		void YearTest();
		void MillisecondTest();
		void SecondTest();
		void MinuteTest();
		void HourTest();
		
		void GetTimeTextTest();
		void DayOfTheWeekTest();

		void ToStringTest();
		
		void GetTimeComponentTest();
		void GetDateInTextFormatTest();
		void GetDateInNumericalFormatTest();

		void CreateTimeTest();

		void BasicCalenadarTest();
	};
}
#endif

