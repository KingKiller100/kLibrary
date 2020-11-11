#include "pch.hpp"
#include "Calendar_Test.hpp"

#include "../../Source Files/Utility/Calendar/kCalendar.hpp"
#include "../../Source Files/Utility/Calendar/kCalenderHelpers.hpp"

#include <Windows.h>


#ifdef TESTING_ENABLED
namespace kTest::utility
{

	CalendarTester::CalendarTester()
		: Tester("Calendar Test")
	{}

	CalendarTester::~CalendarTester()
		= default;

	using namespace klib::kCalendar;

	template<typename T, typename Arg, typename  ...Args>
	std::basic_string<T> MakeString(const T* format, Arg arg, Args ...args)
	{
		T* buffer = nullptr;

		if _CONSTEXPR_IF(std::is_same_v<T, char>)
		{
			const auto length = 1 + std::snprintf(nullptr, 0, format, arg, args...);
			if (length <= 0) throw std::exception();
			buffer = new T[length]{};
			sprintf_s(buffer, length, format, arg, args...);
		}
		else if _CONSTEXPR_IF(std::is_same_v<T, wchar_t>)
		{
			const auto length = 1 + _snwprintf_l(nullptr, 0, format, nullptr, arg, args...);
			if (length <= 0) throw std::exception();
			buffer = new T[length]{};
			swprintf_s(buffer, length, format, arg, args...);
		}

		std::basic_string<T> str = buffer;
		delete[] buffer;
		return str;
	}

	void CalendarTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(DayTest());
		VERIFY_MULTI(YearTest());
		VERIFY_MULTI(MonthTest());
		VERIFY_MULTI(MillisecondTest());
		VERIFY_MULTI(SecondTest());
		VERIFY_MULTI(MinuteTest());
		VERIFY_MULTI(HourTest());
		VERIFY_MULTI(ToStringTest());
		VERIFY_MULTI(CreateTimeTest());
		VERIFY_MULTI(GetTimeTextTest());
		VERIFY_MULTI(DayOfTheWeekTest());
		VERIFY_MULTI(GetTimeComponentTest());
		VERIFY_MULTI(GetDateInTextFormatTest());
		VERIFY_MULTI(GetDateInNumericalFormatTest());
		VERIFY_MULTI_END();
	}

	bool CalendarTester::DayTest()
	{
		{
			constexpr Day day(16, Day::WEDNESDAY);
			VERIFY_COMPILE_TIME(day.GetValue() == 16);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::WEDNESDAY);
			VERIFY(day.Verify());
			VERIFY(day.ToString("d") == "16"
				&& day.ToString("dd") == "16");
			VERIFY(day.ToString("ddd") == "16th");
			VERIFY(day.ToString("dddd") == "Wed");
			VERIFY(day.ToString("ddddd") == "Wednesday");
		}
		
		{
			constexpr Day day(9, Day::SATURDAY);
			VERIFY_COMPILE_TIME(day.GetValue() == 9);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::SATURDAY);
			VERIFY(day.Verify());
			VERIFY(day.ToString("d") == "9"
				&& day.ToString("dd") == "09");
			VERIFY(day.ToString("ddd") == "9th");
			VERIFY(day.ToString("dddd") == "Sat");
			VERIFY(day.ToString("ddddd") == "Saturday");
		}
		
		{
			constexpr Day day(32, Day::THURSDAY);
			VERIFY_COMPILE_TIME(day.GetValue() == 32);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::THURSDAY);
			VERIFY(!day.Verify());
			VERIFY(day.ToString("d") == "32"
				&& day.ToString("dd") == "32");
			VERIFY(day.ToString("ddd") == "32nd");
			VERIFY(day.ToString("dddd") == "Thu");
			VERIFY(day.ToString("ddddd") == "Thursday");
		}
		
		{
			constexpr auto day = 30_d;
			VERIFY_COMPILE_TIME(day.GetValue() == 30);
			VERIFY_COMPILE_TIME(day.GetDayOfTheWeek() == Day::MONDAY);
			VERIFY(day.Verify());
			VERIFY(day.ToString("d") == "30"
				&& day.ToString("dd") == "30");
			VERIFY(day.ToString("ddd") == "30th");
			VERIFY(day.ToString("dddd") == "Mon");
			VERIFY(day.ToString("ddddd") == "Monday");
		}
		
		return success;
	}

	bool CalendarTester::YearTest()
	{
		{
			constexpr Year year(2004);
			VERIFY_COMPILE_TIME(year.IsLeapYear());
			VERIFY_COMPILE_TIME(year.GetValue() == 2004);
			VERIFY_COMPILE_TIME(year.TotalDays() == 366);
			VERIFY(year.ToString("y") == "04"
				&& year.ToString("yy") == "04");
			VERIFY(year.ToString("yyy") == "2004");
			VERIFY(year.ToString("yyyy") == "2004");
		}

		{
			constexpr Year year(1862);
			VERIFY_COMPILE_TIME(!year.IsLeapYear());
			VERIFY_COMPILE_TIME(year.GetValue() == 1862);
			VERIFY_COMPILE_TIME(year.TotalDays() == 365);
			VERIFY(year.ToString("y") == "62"
				&& year.ToString("yy") == "62");
			VERIFY(year.ToString("yyy") == "1862");
			VERIFY(year.ToString("yyyy") == "1862");
		}

		{
			constexpr auto year = 2000_y;
			VERIFY_COMPILE_TIME(year.IsLeapYear());
			VERIFY_COMPILE_TIME(year.GetValue() == 2000);
			VERIFY_COMPILE_TIME(year.TotalDays() == 366);
			VERIFY(year.ToString("y") == "00"
				&& year.ToString("yy") == "00");
			VERIFY(year.ToString("yyy") == "2000");
			VERIFY(year.ToString("yyyy") == "2000");
		}

		{
			constexpr auto yearsFromDays = static_cast<float>(YearsFromDays(1000));
			VERIFY_COMPILE_TIME(yearsFromDays == 2.739726f)
		}

		{
			constexpr auto yearsFromDays = YearsFromDays(1'461);
			VERIFY_COMPILE_TIME(yearsFromDays == 4)
		}

		{
			constexpr auto yearsFromMonths = YearsFromMonths(240);
			VERIFY_COMPILE_TIME(yearsFromMonths == 20);
		}

		return success;
	}

	bool CalendarTester::MonthTest()
	{
		{
			constexpr Month month(Month::MAR);
			constexpr Day day(4);
			constexpr Year year(1998);
			VERIFY_COMPILE_TIME(month.GetMonthNumber() == 3);
			VERIFY_COMPILE_TIME(month.GetValue() == Month::MAR);
			VERIFY(month.Verify(day, year));
			VERIFY(month.ToString("m") == "3");
			VERIFY(month.ToString("mm") == "03");
			VERIFY(month.ToString("mmm") == "Mar");
			VERIFY(month.ToString("mmmm") == "March");
		}

		{
			constexpr Month month(Month::DEC);
			constexpr Day day(20);
			constexpr Year year(1999);
			VERIFY_COMPILE_TIME(month.GetMonthNumber() == 12);
			VERIFY_COMPILE_TIME(month.GetValue() == Month::DEC);
			VERIFY(month.Verify(day, year));
			VERIFY(month.ToString("m") == "12"
				&& month.ToString("mm") == "12");
			VERIFY(month.ToString("mmm") == "Dec");
			VERIFY(month.ToString("mmmm") == "December");
		}

		{
			constexpr auto month = 2_m;
			constexpr auto day = 29_d;
			constexpr auto year = 1999_y;
			VERIFY_COMPILE_TIME(month.GetMonthNumber() == 2);
			VERIFY_COMPILE_TIME(month.GetValue() == Month::FEB);
			VERIFY(!month.Verify(day, year));
			VERIFY(month.ToString("m") == "2"
				&& month.ToString("mm") == "02");
			VERIFY(month.ToString("mmm") == "Feb");
			VERIFY(month.ToString("mmmm") == "February");
		}

		{
			constexpr auto daysCount = 5150;
			constexpr auto monthFromDays = MonthsFromDays(daysCount);
			constexpr auto divider = 30.4167;
			constexpr auto expected = daysCount / divider;
			VERIFY_COMPILE_TIME(monthFromDays == expected);
		}

		return success;
	}

	bool CalendarTester::MillisecondTest()
	{
		{
			constexpr Millisecond millis(300);
			VERIFY_COMPILE_TIME(millis.GetValue() == 300);
			VERIFY(millis.Verify());
			VERIFY(millis.ToString("c") == "300");
			VERIFY(millis.ToString("cc") == "300");
			VERIFY(millis.ToString("ccc") == "300");
			VERIFY(millis.ToString("cccc") == "300ms");
			VERIFY(millis.ToString("ccccc") == "300ms");
		}
		
		{
			constexpr Millisecond millis { 3 };
			VERIFY_COMPILE_TIME(millis.GetValue() == 3);
			VERIFY(millis.Verify());
			VERIFY(millis.ToString("c") == "3");
			VERIFY(millis.ToString("cc") == "03");
			VERIFY(millis.ToString("ccc") == "003");
			VERIFY(millis.ToString("cccc") == "3ms");
			VERIFY(millis.ToString("ccccc") == "003ms");
		}
		
		{
			constexpr Millisecond millis(75);
			VERIFY_COMPILE_TIME(millis.GetValue() == 75);
			VERIFY(millis.Verify());
			VERIFY(millis.ToString("c") == "75");
			VERIFY(millis.ToString("cc") == "75");
			VERIFY(millis.ToString("ccc") == "075");
			VERIFY(millis.ToString("cccc") == "75ms");
			VERIFY(millis.ToString("ccccc") == "075ms");
		}

		{
			using namespace std::chrono;
			using namespace std::chrono_literals;
			const auto result = TimeConverter<Second>(500'000_ms);
			constexpr auto expected = duration_cast<seconds>(500'000ms);
			VERIFY(expected.count() == result.GetValue());
		}

		{
			using namespace std::chrono;
			using namespace std::chrono_literals;
			const auto result = TimeConverter<Minute>(500'000_ms);
			constexpr auto expected = duration_cast<minutes>(500'000ms);
			VERIFY(expected.count() == result.GetValue());
		}

		{
			using namespace std::chrono;
			using namespace std::chrono_literals;
			const auto result = TimeConverter<Hour>(500'000_ms);
			constexpr auto expected = duration_cast<hours>(500'000ms);
			VERIFY(expected.count() == result.GetValue());
		}

		return success;
	}

	bool CalendarTester::SecondTest()
	{
		{
			constexpr Second sec(30);
			VERIFY_COMPILE_TIME(sec.GetValue() == 30);
			VERIFY(sec.Verify());
			VERIFY(sec.ToString("s") == "30");
			VERIFY(sec.ToString("ss") == "30");
			VERIFY(sec.ToString("sss") == "30s");
			VERIFY(sec.ToString("ssss") == "30s");
		}
		
		{
			constexpr Second sec(4);
			VERIFY_COMPILE_TIME(sec.GetValue() == 4);
			VERIFY(sec.Verify());
			VERIFY(sec.ToString("s") == "4");
			VERIFY(sec.ToString("ss") == "04");
			VERIFY(sec.ToString("sss") == "4s");
			VERIFY(sec.ToString("ssss") == "04s");
		}
		
		{
			constexpr auto sec(100_ss);
			VERIFY_COMPILE_TIME(sec.GetValue() == 100);
			VERIFY(!sec.Verify());
			VERIFY(sec.ToString("s") == "100");
			VERIFY(sec.ToString("ss") == "100");
			VERIFY(sec.ToString("sss") == "100s");
			VERIFY(sec.ToString("ssss") == "100s");
		}
		
		return success;
	}

	bool CalendarTester::MinuteTest()
	{
		{
			constexpr Minute minute(30);
			VERIFY_COMPILE_TIME(minute.GetValue() == 30);
			VERIFY(minute.Verify());
			VERIFY(minute.ToString("m") == "30");
			VERIFY(minute.ToString("mm") == "30");
			VERIFY(minute.ToString("mmm") == "30m");
			VERIFY(minute.ToString("mmmm") == "30m");
		}
		
		{
			constexpr Minute minute(1);
			VERIFY_COMPILE_TIME(minute.GetValue() == 1);
			VERIFY(minute.Verify());
			VERIFY(minute.ToString("m") == "1");
			VERIFY(minute.ToString("mm") == "01");
			VERIFY(minute.ToString("mmm") == "1m");
			VERIFY(minute.ToString("mmmm") == "01m");
		}
		
		{
			constexpr auto minute(1000_mm);
			VERIFY_COMPILE_TIME(minute.GetValue() == 1000);
			VERIFY(!minute.Verify());
			VERIFY(minute.ToString("m") == "1000");
			VERIFY(minute.ToString("mm") == "1000");
			VERIFY(minute.ToString("mmm") == "1000m");
			VERIFY(minute.ToString("mmmm") == "1000m");
		}
		
		return success;
	}

	bool CalendarTester::HourTest()
	{
		{
			constexpr Hour hour(12);
			VERIFY_COMPILE_TIME(hour.GetValue() == 12);
			VERIFY(hour.Verify());
			VERIFY(hour.ToString("h") == "12");
			VERIFY(hour.ToString("hh") == "12");
			VERIFY(hour.ToString("hhh") == "12m");
			VERIFY(hour.ToString("hhhh") == "12m");
		}
		
		{
			constexpr Hour hour(2);
			VERIFY_COMPILE_TIME(hour.GetValue() == 2);
			VERIFY(hour.Verify());
			VERIFY(hour.ToString("h") == "2");
			VERIFY(hour.ToString("hh") == "02");
			VERIFY(hour.ToString("hhh") == "2m");
			VERIFY(hour.ToString("hhhh") == "02m");
		}
		
		{
			constexpr auto hour(30_hh);
			VERIFY_COMPILE_TIME(hour.GetValue() == 30);
			VERIFY(!hour.Verify());
			VERIFY(hour.ToString("h") == "30");
			VERIFY(hour.ToString("hh") == "30");
			VERIFY(hour.ToString("hhh") == "30m");
			VERIFY(hour.ToString("hhhh") == "30m");
		}
		
		return success;
	}

	auto DateTextFunc(const SYSTEMTIME dateTime, Date::DateTextLength format = Date::DateTextLength::FULL) -> decltype(auto)
	{
		constexpr std::array<const char*, 12> months = {
			"January", "February", "March",
			"April", "May", "June",
			"July", "August", "September",
			"October", "November", "December"
		};

		constexpr  std::array<const char*, 7> days = {
			"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"
		};

		const std::string day = days[dateTime.wDayOfWeek];
		std::string month = months[dateTime.wMonth - 1];

		const auto dateSuffix = [&]()
		{
			return (dateTime.wDay == 1 || dateTime.wDay == 21 || dateTime.wDay == 31) ? "st"
				: (dateTime.wDay == 2 || dateTime.wDay == 22) ? "nd"
				: dateTime.wDay == 3 ? "rd"
				: "th";
		};

		std::string dateStr;
		if (format == Date::FULL)
		{
			dateStr = MakeString<char>("%s %d%s %s %04d",
				day.data(),
				dateTime.wDay,
				dateSuffix(),
				month.data(),
				dateTime.wYear);
		}
		else
		{

			dateStr = MakeString<char>("%s %d%s %d",
				month.data(),
				dateTime.wDay,
				dateSuffix(),
				dateTime.wYear);
		}

		return dateStr;
	}

	bool CalendarTester::GetTimeTextTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetTimeText();
			const auto expected = MakeString("%02d:%02d:%02d:%03d",
				localTime.wHour,
				localTime.wMinute,
				localTime.wSecond,
				localTime.wMilliseconds);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = klib::kString::Convert<wchar_t>(GetTimeText());
			const auto expected = MakeString(L"%02d:%02d:%02d:%03d",
				localTime.wHour,
				localTime.wMinute,
				localTime.wSecond,
				localTime.wMilliseconds);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetTimeText(CalendarInfoSourceType::SYSTEM);
			const auto expected = MakeString("%02d:%02d:%02d:%03d",
				systemTime.wHour,
				systemTime.wMinute,
				systemTime.wSecond,
				systemTime.wMilliseconds);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = klib::kString::Convert<wchar_t>(GetTimeText(CalendarInfoSourceType::SYSTEM));
			const auto expected = MakeString(L"%02d:%02d:%02d:%03d",
				systemTime.wHour,
				systemTime.wMinute,
				systemTime.wSecond,
				systemTime.wMilliseconds);
			VERIFY(result == expected);
		}


		return success;
	}

	bool CalendarTester::DayOfTheWeekTest()
	{
		{
			constexpr auto dotw = Day::TUESDAY;
			constexpr auto result = Day::DayOfTheWeekToString(dotw);
			VERIFY_COMPILE_TIME(result == "Tuesday");
		}

		{
			constexpr auto dotw = Day::FRIDAY;
			constexpr auto result = Day::DayOfTheWeekToString(dotw);
			VERIFY_COMPILE_TIME(result == "Friday");
		}

		{
			constexpr auto dotw = Day::SUNDAY;
			constexpr auto result = Day::DayOfTheWeekToString(dotw);
			VERIFY_COMPILE_TIME(result == "Sunday");
		}

		return success;
	}

	bool CalendarTester::ToStringTest()
	{
		{
			const Date date(Day::MONDAY, 1, Month::JAN, 1998);
			const auto result = date.ToString("dd/mm/yy");
			const char expected[] = "01/01/98";
			VERIFY(expected == result);
		}

		{
			const Date date(Day::WEDNESDAY, 1, Month::APR, 2004);
			const auto result = date.ToString("dddd, ddd mmmm yyyy");
			const char expected[] = "Wed, 1st Apr 2004";
			VERIFY(expected == result);
		}

		{
			const Date date(Day::SATURDAY, 18, Month::JUL, 3005);
			const auto result = date.ToString("ddddd, ddd");
			const char expected[] = "Saturday, 18th";
			VERIFY(expected == result);
		}

		{
			const Time t(12, 5, 20);
			const auto result = t.ToString("hh:mm:ss");
			const char expected[] = "12:05:20";
			VERIFY(expected == result);
		}

		{
			const Time t(23, 10, 50);
			const auto result = t.ToString("hh:mm");
			const char expected[] = "23:10";
			VERIFY(expected == result);
		}

		{
			const Time t(23, 10, 50, 34);
			const auto result = t.ToString("ccc");
			const char expected[] = "034";
			VERIFY(expected == result);
		}

		return success;
	}

	bool CalendarTester::GetTimeComponentTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto hour = GetComponentOfTime(Time::HOURS);
			VERIFY(hour == localTime.wHour);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto mins = GetComponentOfTime(Time::MINS);
			VERIFY(mins == localTime.wMinute);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto secs = GetComponentOfTime(Time::SECS);
			VERIFY(secs == localTime.wSecond);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);

			const auto millis = GetComponentOfTime(Time::MILLIS);
			VERIFY(millis == localTime.wMilliseconds);
		}


		return success;
	}



	bool CalendarTester::GetDateInTextFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::FULL);
			const auto expected = DateTextFunc(localTime);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::SHORT);
			const auto expected = DateTextFunc(localTime, Date::DateTextLength::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInTextFormat(Date::SHORT, CalendarInfoSourceType::SYSTEM);
			const auto expected = DateTextFunc(systemTime, Date::DateTextLength::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::DateTextLength::FULL);
			const auto expected = DateTextFunc(localTime);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInTextFormat(Date::DateTextLength::SHORT);
			const auto expected = DateTextFunc(localTime, Date::DateTextLength::SHORT);
			VERIFY(result == expected);
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInTextFormat(Date::SHORT, CalendarInfoSourceType::SYSTEM);
			const auto expected = DateTextFunc(systemTime, Date::SHORT);
			VERIFY(result == expected);
		}


		return success;
	}

	bool CalendarTester::GetDateInNumericalFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = klib::kString::Convert<wchar_t>(GetDateInNumericalFormat(Date::SLASH));
			const auto expected = klib::kString::ToString(L"%02d/%02d/%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			const auto result = GetDateInNumericalFormat(Date::DASH);
			const auto expected = klib::kString::ToString("%02d-%02d-%02d", localTime.wDay, localTime.wMonth, localTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result = GetDateInNumericalFormat(Date::SLASH, CalendarInfoSourceType::SYSTEM);
			const auto expected = klib::kString::ToString("%02d/%02d/%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime(&systemTime);
			const auto result
				= klib::kString::Convert<wchar_t>(GetDateInNumericalFormat(Date::DASH, CalendarInfoSourceType::SYSTEM));
			const auto expected = klib::kString::ToString(L"%02d-%02d-%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear);
			VERIFY(result == expected)
		}


		return success;
	}

	bool CalendarTester::CreateTimeTest()
	{
		{
			const auto result = CreateTime(12, 30, 25);
			constexpr const auto* const expected = "12:30:25";
			VERIFY(result == expected);
		}

		{
			const auto result = CreateTime(16, 18, 0, 567);
			constexpr const auto* const expected = "16:18:00:567";
			VERIFY(result == expected);
		}

		{
			const auto result = CreateTime(6, 2, 8, 24);
			constexpr const auto* const expected = "06:02:08:024";
			VERIFY(result == expected);
		}

		{
			const auto result = CreateTime(6, 20);
			constexpr const auto* const expected = "06:20";
			VERIFY(result == expected);
		}

		{
			const auto result = CreateTime(24, 60, 60);

			constexpr const auto* const bad = "24:60:60";
			VERIFY(result != bad);
			constexpr const auto* const expected = "23:59:59";
			VERIFY(result == expected);
		}

		return success;
	}
}
#endif

