#include "Calendar_Test.hpp"

#include "../../Source/Utility/Calendar/kCalendar.hpp"
#include "../../Source/Utility/Calendar/kCalenderHelpers.hpp"
#include "../../Source/Utility/String/kToString.hpp"


#include <Windows.h>


#ifdef TESTING_ENABLED
namespace kTest::utility
{
	CalendarTester::CalendarTester()
		: TesterBase( "Calendar Test" )
	{}

	CalendarTester::~CalendarTester()
	= default;

	using namespace klib::kCalendar;

	template <typename T, typename Arg, typename ...Args>
	std::basic_string<T> MakeString( const T* format, Arg arg, Args ...args )
	{
		T* buffer = nullptr;

		if constexpr ( std::is_same_v<T, char> )
		{
			const auto length = 1 + std::snprintf( nullptr, 0, format, arg, args... );
			if ( length <= 0 )
				throw std::exception();
			buffer = new T[length]{};
			sprintf_s( buffer, length, format, arg, args... );
		}
		else if constexpr ( std::is_same_v<T, wchar_t> )
		{
			const auto length = 1 + _snwprintf_l( nullptr, 0, format, nullptr, arg, args... );
			if ( length <= 0 )
				throw std::exception();
			buffer = new T[length]{};
			swprintf_s( buffer, length, format, arg, args... );
		}

		std::basic_string<T> str = buffer;
		delete[] buffer;
		return str;
	}

	void CalendarTester::Prepare() noexcept
	{
		ADD_TEST( DayTest() );
		ADD_TEST( YearTest() );
		ADD_TEST( MonthTest() );
		ADD_TEST( MillisecondTest() );
		ADD_TEST( SecondTest() );
		ADD_TEST( MinuteTest() );
		ADD_TEST( HourTest() );
		ADD_TEST( ToStringTest() );
		ADD_TEST( CreateTimeTest() );
		ADD_TEST( GetTimeTextTest() );
		ADD_TEST( DayOfTheWeekTest() );
		ADD_TEST( GetTimeComponentTest() );
		ADD_TEST( GetDateInTextFormatTest() );
		ADD_TEST( GetDateInNumericalFormatTest() );
		ADD_TEST( BasicCalenadarTest() );
	}

	void CalendarTester::DayTest()
	{
		{
			constexpr Day day( 16, Day::WEDNESDAY );
			VERIFY_COMPILE_TIME( day.GetValue() == 16 );
			VERIFY_COMPILE_TIME( day.GetDayOfTheWeek() == Day::WEDNESDAY );
			VERIFY( day.Verify() );
			VERIFY( day.ToString("d") == "16"
				&& day.ToString("dd") == "16" );
			VERIFY( day.ToString("ddd") == "16th" );
			VERIFY( day.ToString("dddd") == "Wed" );
			VERIFY( day.ToString("ddddd") == "Wednesday" );
		}

		{
			const Day day( 9, Day::SATURDAY );
			VERIFY( day.GetValue() == 9 );
			VERIFY( day.GetDayOfTheWeek() == Day::SATURDAY );
			VERIFY( day.Verify() );
			VERIFY( day.ToString("d") == "9"
				&& day.ToString("dd") == "09" );
			VERIFY( day.ToString("ddd") == "9th" );
			VERIFY( day.ToString("dddd") == "Sat" );
			VERIFY( day.ToString("ddddd") == "Saturday" );
		}

		{
			const Day day( 32, Day::THURSDAY );
			VERIFY( day.GetValue() == 32 );
			VERIFY( day.GetDayOfTheWeek() == Day::THURSDAY );
			VERIFY( !day.Verify() );
			VERIFY( day.ToString("d") == "32"
				&& day.ToString("dd") == "32" );
			VERIFY( day.ToString("ddd") == "32nd" );
			VERIFY( day.ToString("dddd") == "Thu" );
			VERIFY( day.ToString("ddddd") == "Thursday" );
		}

		{
			const Day day = 30;
			VERIFY( day.GetValue() == 30 );
			VERIFY( day.GetDayOfTheWeek() == Day::MONDAY );
			VERIFY( day.Verify() );
			VERIFY( day.ToString("d") == "30"
				&& day.ToString("dd") == "30" );
			VERIFY( day.ToString("ddd") == "30th" );
			VERIFY( day.ToString("dddd") == "Mon" );
			VERIFY( day.ToString("ddddd") == "Monday" );
		}
	}

	void CalendarTester::YearTest()
	{
		{
			constexpr Year year( 2004 );
			VERIFY_COMPILE_TIME( year.IsLeapYear() );
			VERIFY_COMPILE_TIME( year.GetValue() == 2004 );
			VERIFY_COMPILE_TIME( year.TotalDays() == 366 );
			VERIFY( year.ToString("y") == "04"
				&& year.ToString("yy") == "04" );
			VERIFY( year.ToString("yyy") == "2004" );
			VERIFY( year.ToString("yyyy") == "2004" );
		}

		{
			const Year year( 1862 );
			VERIFY( !year.IsLeapYear() );
			VERIFY( year.GetValue() == 1862 );
			VERIFY( year.TotalDays() == 365 );
			VERIFY( year.ToString("y") == "62"
				&& year.ToString("yy") == "62" );
			VERIFY( year.ToString("yyy") == "1862" );
			VERIFY( year.ToString("yyyy") == "1862" );
		}

		{
			constexpr Year year{ 2000 };
			VERIFY( year.IsLeapYear() );
			VERIFY( year.GetValue() == 2000 );
			VERIFY( year.TotalDays() == 366 );
			VERIFY( year.ToString("y") == "00"
				&& year.ToString("yy") == "00" );
			VERIFY( year.ToString("yyy") == "2000" );
			VERIFY( year.ToString("yyyy") == "2000" );
		}

		{
			constexpr auto yearsFromDays = static_cast<float>( YearsFromDays( 1000 ) );
			VERIFY_COMPILE_TIME( yearsFromDays == 2.739726f )
		}

		{
			constexpr auto yearsFromDays = YearsFromDays( 1'461 );
			VERIFY_COMPILE_TIME( yearsFromDays == 4 )
		}

		{
			constexpr auto yearsFromMonths = YearsFromMonths( 240 );
			VERIFY_COMPILE_TIME( yearsFromMonths == 20 );
		}
	}

	void CalendarTester::MonthTest()
	{
		{
			constexpr Month month( Month::MAR );
			constexpr Day day( 4 );
			constexpr Year year( 1998 );
			VERIFY_COMPILE_TIME( month.GetMonthNumber() == 3 );
			VERIFY_COMPILE_TIME( month.GetValue() == Month::MAR );
			VERIFY( month.Verify(day, year) );
			VERIFY( month.ToString("m") == "3" );
			VERIFY( month.ToString("mm") == "03" );
			VERIFY( month.ToString("mmm") == "Mar" );
			VERIFY( month.ToString("mmmm") == "March" );
		}

		{
			const Month month( Month::DEC );
			const Day day( 20 );
			const Year year( 1999 );
			VERIFY( month.GetMonthNumber() == 12 );
			VERIFY( month.GetValue() == Month::DEC );
			VERIFY( month.Verify(day, year) );
			VERIFY( month.ToString("m") == "12"
				&& month.ToString("mm") == "12" );
			VERIFY( month.ToString("mmm") == "Dec" );
			VERIFY( month.ToString("mmmm") == "December" );
		}

		{
			const Month month{ Month::FEB };
			const Day day{ 29 };
			const Year year{ 1999 };
			VERIFY( month.GetMonthNumber() == 2 );
			VERIFY( month.GetValue() == Month::FEB );
			VERIFY( !month.Verify(day, year) );
			VERIFY( month.ToString("m") == "2"
				&& month.ToString("mm") == "02" );
			VERIFY( month.ToString("mmm") == "Feb" );
			VERIFY( month.ToString("mmmm") == "February" );
		}

		{
			constexpr auto daysCount = 5150;
			constexpr auto monthFromDays = MonthsFromDays( daysCount );
			constexpr auto divider = 30.4167;
			constexpr auto expected = daysCount / divider;
			VERIFY_COMPILE_TIME( monthFromDays == expected );
		}
	}

	void CalendarTester::MillisecondTest()
	{
		{
			const Millisecond millis( 300 );
			VERIFY( millis.GetValue() == 300 );
			VERIFY( millis.Verify() );
			VERIFY( millis.ToString("c") == "300" );
			VERIFY( millis.ToString("cc") == "300" );
			VERIFY( millis.ToString("ccc") == "300" );
			VERIFY( millis.ToString("cccc") == "300ms" );
			VERIFY( millis.ToString("ccccc") == "300ms" );
		}

		{
			const Millisecond millis{3};
			VERIFY( millis.GetValue() == 3 );
			VERIFY( millis.Verify() );
			VERIFY( millis.ToString("c") == "3" );
			VERIFY( millis.ToString("cc") == "03" );
			VERIFY( millis.ToString("ccc") == "003" );
			VERIFY( millis.ToString("cccc") == "3ms" );
			VERIFY( millis.ToString("ccccc") == "003ms" );
		}

		{
			const Millisecond millis( 75 );
			VERIFY( millis.GetValue() == 75 );
			VERIFY( millis.Verify() );
			VERIFY( millis.ToString("c") == "75" );
			VERIFY( millis.ToString("cc") == "75" );
			VERIFY( millis.ToString("ccc") == "075" );
			VERIFY( millis.ToString("cccc") == "75ms" );
			VERIFY( millis.ToString("ccccc") == "075ms" );
		}
	}

	void CalendarTester::SecondTest()
	{
		{
			const Second sec( 30 );
			VERIFY( sec.GetValue() == 30 );
			VERIFY( sec.Verify() );
			VERIFY( sec.ToString("s") == "30" );
			VERIFY( sec.ToString("ss") == "30" );
			VERIFY( sec.ToString("sss") == "30s" );
			VERIFY( sec.ToString("ssss") == "30s" );
		}

		{
			constexpr auto size = sizeof Second;
			const Second sec( 4 );
			VERIFY( sec.GetValue() == 4 );
			VERIFY( sec.Verify() );
			VERIFY( sec.ToString("s") == "4" );
			VERIFY( sec.ToString("ss") == "04" );
			VERIFY( sec.ToString("sss") == "4s" );
			VERIFY( sec.ToString("ssss") == "04s" );
		}

		{
			constexpr Second sec( 100 );
			VERIFY( sec.GetValue() == 100 );
			VERIFY( !sec.Verify() );
			VERIFY( sec.ToString("s") == "100" );
			VERIFY( sec.ToString("ss") == "100" );
			VERIFY( sec.ToString("sss") == "100s" );
			VERIFY( sec.ToString("ssss") == "100s" );
		}
	}

	void CalendarTester::MinuteTest()
	{
		{
			const Minute minute( 30 );
			VERIFY( minute.GetValue() == 30 );
			VERIFY( minute.Verify() );
			VERIFY( minute.ToString("m") == "30" );
			VERIFY( minute.ToString("mm") == "30" );
			VERIFY( minute.ToString("mmm") == "30m" );
			VERIFY( minute.ToString("mmmm") == "30m" );
		}

		{
			const Minute minute( 1 );
			VERIFY( minute.GetValue() == 1 );
			VERIFY( minute.Verify() );
			VERIFY( minute.ToString("m") == "1" );
			VERIFY( minute.ToString("mm") == "01" );
			VERIFY( minute.ToString("mmm") == "1m" );
			VERIFY( minute.ToString("mmmm") == "01m" );
		}

		{
			constexpr Minute minute( 1000 );
			VERIFY( minute.GetValue() == 1000 );
			VERIFY( !minute.Verify() );
			VERIFY( minute.ToString("m") == "1000" );
			VERIFY( minute.ToString("mm") == "1000" );
			VERIFY( minute.ToString("mmm") == "1000m" );
			VERIFY( minute.ToString("mmmm") == "1000m" );
		}
	}

	void CalendarTester::HourTest()
	{
		{
			const Hour hour( 12 );
			VERIFY( hour.GetValue() == 12 );
			VERIFY( hour.Verify() );
			VERIFY( hour.ToString("h") == "12" );
			VERIFY( hour.ToString("hh") == "12" );
			VERIFY( hour.ToString("hhh") == "12h" );
			VERIFY( hour.ToString("hhhh") == "12h" );
		}

		{
			const Hour hour( 2 );
			VERIFY( hour.GetValue() == 2 );
			VERIFY( hour.Verify() );
			VERIFY( hour.ToString("h") == "2" );
			VERIFY( hour.ToString("hh") == "02" );
			VERIFY( hour.ToString("hhh") == "2h" );
			VERIFY( hour.ToString("hhhh") == "02h" );
		}

		{
			constexpr Hour hour( 30 );
			VERIFY( hour.GetValue() == 30 );
			VERIFY( !hour.Verify() );
			VERIFY( hour.ToString("h") == "30" );
			VERIFY( hour.ToString("hh") == "30" );
			VERIFY( hour.ToString("hhh") == "30h" );
			VERIFY( hour.ToString("hhhh") == "30h" );
		}
	}

	auto DateTextFunc( const SYSTEMTIME dateTime, Date::DateTextLength format = Date::DateTextLength::FULL ) -> decltype(auto)
	{
		constexpr std::array<const char*, 12> months = {
			"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
		};

		constexpr std::array<const char*, 7> days = {
			"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
		};

		const std::string day = days[dateTime.wDayOfWeek];
		std::string month = months[dateTime.wMonth - 1];

		const auto dateSuffix = [&]()
		{
			const auto d = dateTime.wDay;

			return ( d % 10 == 1 && ( d > 20 || d < 10 ) )
				       ? "st"
				       : ( d % 10 == 2 && ( d > 20 || d < 10 ) )
				       ? "nd"
				       : ( d % 10 == 3 && ( d > 20 || d < 10 ) )
				       ? "rd"
				       : "th";
		};

		std::string dateStr;
		if ( format == Date::FULL )
		{
			dateStr = MakeString<char>( "%s %d%s %s %04d",
				day.data(),
				dateTime.wDay,
				dateSuffix(),
				month.data(),
				dateTime.wYear );
		}
		else
		{
			dateStr = MakeString<char>( "%s %d%s %d",
				month.data(),
				dateTime.wDay,
				dateSuffix(),
				dateTime.wYear );
		}

		return dateStr;
	}

	void CalendarTester::GetTimeTextTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = GetTimeText();
			const auto expected = MakeString( "%02d:%02d:%02d:%03d",
				localTime.wHour,
				localTime.wMinute,
				localTime.wSecond,
				localTime.wMilliseconds );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = klib::kString::Convert<wchar_t>( GetTimeText() );
			const auto expected = MakeString( L"%02d:%02d:%02d:%03d",
				localTime.wHour,
				localTime.wMinute,
				localTime.wSecond,
				localTime.wMilliseconds );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime( &systemTime );
			const auto result = GetTimeText( CalendarInfoSourceType::SYSTEM );
			const auto expected = MakeString( "%02d:%02d:%02d:%03d",
				systemTime.wHour,
				systemTime.wMinute,
				systemTime.wSecond,
				systemTime.wMilliseconds );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime( &systemTime );
			const auto result = klib::kString::Convert<wchar_t>( GetTimeText( CalendarInfoSourceType::SYSTEM ) );
			const auto expected = MakeString( L"%02d:%02d:%02d:%03d",
				systemTime.wHour,
				systemTime.wMinute,
				systemTime.wSecond,
				systemTime.wMilliseconds );
			VERIFY( result == expected );
		}
	}

	void CalendarTester::DayOfTheWeekTest()
	{
		{
			constexpr auto dotw = Day::TUESDAY;
			constexpr auto result = Day::DayOfTheWeekToString( dotw );
			VERIFY_COMPILE_TIME( result == "Tuesday" );
		}

		{
			constexpr auto dotw = Day::FRIDAY;
			constexpr auto result = Day::DayOfTheWeekToString( dotw );
			VERIFY_COMPILE_TIME( result == "Friday" );
		}

		{
			constexpr auto dotw = Day::SUNDAY;
			constexpr auto result = Day::DayOfTheWeekToString( dotw );
			VERIFY_COMPILE_TIME( result == "Sunday" );
		}
	}

	void CalendarTester::ToStringTest()
	{
		{
			const Date date( Day::MONDAY, 1, Month::JAN, 1998 );
			const auto result = date.ToString( "dd/mm/yy" );
			const char expected[] = "01/01/98";
			VERIFY( expected == result );
		}

		{
			const Date date( Day::WEDNESDAY, 1, Month::APR, 2004 );
			const auto result = date.ToString( "dddd, ddd mmm yyyy" );
			const char expected[] = "Wed, 1st Apr 2004";
			VERIFY( expected == result );
		}

		{
			const Date date( Day::SATURDAY, 18, Month::JUL, 3005 );
			const auto result = date.ToString( "ddddd, ddd" );
			const char expected[] = "Saturday, 18th";
			VERIFY( expected == result );
		}

		{
			const Time t( 12, 5, 20 );
			const auto result = t.ToString( "hh:mm:ss" );
			const char expected[] = "12:05:20";
			VERIFY( expected == result );
		}

		{
			const Time t( 23, 10, 50 );
			const auto result = t.ToString( "hh:mm" );
			const char expected[] = "23:10";
			VERIFY( expected == result );
		}

		{
			const Time t( 23, 10, 50, 34 );
			const auto result = t.ToString( "ccc" );
			const char expected[] = "034";
			VERIFY( expected == result );
		}
	}

	void CalendarTester::GetTimeComponentTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );

			const auto hour = GetComponentOfTime( Time::HOURS );
			VERIFY( hour == localTime.wHour );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );

			const auto mins = GetComponentOfTime( Time::MINS );
			VERIFY( mins == localTime.wMinute );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );

			const auto secs = GetComponentOfTime( Time::SECS );
			VERIFY( secs == localTime.wSecond );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );

			const auto millis = GetComponentOfTime( Time::MILLIS );
			VERIFY( millis == localTime.wMilliseconds );
		}
	}


	void CalendarTester::GetDateInTextFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = GetDateInTextFormat( Date::FULL );
			const auto expected = DateTextFunc( localTime );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = GetDateInTextFormat( Date::SHORT );
			const auto expected = DateTextFunc( localTime, Date::DateTextLength::SHORT );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime( &systemTime );
			const auto result = GetDateInTextFormat( Date::SHORT, CalendarInfoSourceType::SYSTEM );
			const auto expected = DateTextFunc( systemTime, Date::DateTextLength::SHORT );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = GetDateInTextFormat( Date::DateTextLength::FULL );
			const auto expected = DateTextFunc( localTime );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = GetDateInTextFormat( Date::DateTextLength::SHORT );
			const auto expected = DateTextFunc( localTime, Date::DateTextLength::SHORT );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime( &systemTime );
			const auto result = GetDateInTextFormat( Date::SHORT, CalendarInfoSourceType::SYSTEM );
			const auto expected = DateTextFunc( systemTime, Date::SHORT );
			VERIFY( result == expected );
		}
	}

	void CalendarTester::GetDateInNumericalFormatTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = klib::kString::Convert<wchar_t>( GetDateInNumericalFormat( Date::SLASH ) );
			const auto expected = klib::kString::Sprintf( L"%02d/%02d/%02d", localTime.wDay, localTime.wMonth, localTime.wYear );

			VERIFY( result == expected );
		}

		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const auto result = GetDateInNumericalFormat( Date::DASH );
			const auto expected = klib::kString::Sprintf( "%02d-%02d-%02d", localTime.wDay, localTime.wMonth, localTime.wYear );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime( &systemTime );
			const auto result = GetDateInNumericalFormat( Date::SLASH, CalendarInfoSourceType::SYSTEM );
			const auto expected = klib::kString::Sprintf( "%02d/%02d/%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear );
			VERIFY( result == expected );
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime( &systemTime );
			const auto result
				= klib::kString::Convert<wchar_t>( GetDateInNumericalFormat( Date::DASH, CalendarInfoSourceType::SYSTEM ) );
			const auto expected = klib::kString::Sprintf( L"%02d-%02d-%02d", systemTime.wDay, systemTime.wMonth, systemTime.wYear );
			VERIFY( result == expected );
		}
	}

	void CalendarTester::CreateTimeTest()
	{
		{
			const auto result = CreateTime( 12, 30, 25 );
			constexpr const auto* const expected = "12:30:25";
			VERIFY( result == expected );
		}

		{
			const auto result = CreateTime( 16, 18, 0, 567 );
			constexpr const auto* const expected = "16:18:00:567";
			VERIFY( result == expected );
		}

		{
			const auto result = CreateTime( 6, 2, 8, 24 );
			constexpr const auto* const expected = "06:02:08:024";
			VERIFY( result == expected );
		}

		{
			const auto result = CreateTime( 6, 20 );
			constexpr const auto* const expected = "06:20";
			VERIFY( result == expected );
		}

		{
			const auto result = CreateTime( 24, 60, 60 );

			constexpr const auto* const bad = "24:60:60";
			VERIFY( result != bad );
			constexpr const auto* const expected = "23:59:59";
			VERIFY( result == expected );
		}
	}

	void CalendarTester::BasicCalenadarTest()
	{
		{
			SYSTEMTIME localTime;
			::GetLocalTime( &localTime );
			const GregorianCalendar calendar( CalendarInfoSourceType::LOCAL );

			const auto& d = calendar.GetDate();
			const auto daysMatch = d.GetDay() == localTime.wDay;
			const auto dotwMatch = d.GetDay().GetDayOfTheWeek() == localTime.wDayOfWeek;
			const auto monthMatch = d.GetMonth() == localTime.wMonth;
			const auto yearMatch = localTime.wYear == d.GetYear();

			VERIFY( daysMatch );
			VERIFY( dotwMatch );
			VERIFY( monthMatch );
			VERIFY( yearMatch );

			const auto& t = calendar.GetTime();
			const auto hourMatch = t.GetHour() == localTime.wHour;
			const auto minMatch = t.GetMinute() == localTime.wMinute;
			const auto secMatch = t.GetSecond() == localTime.wSecond;
			const auto milliMatch = t.GetMillisecond() == localTime.wMilliseconds;

			VERIFY( hourMatch );
			VERIFY( minMatch );
			VERIFY( secMatch );
			VERIFY( milliMatch );
		}

		{
			SYSTEMTIME systemTime;
			::GetSystemTime( &systemTime );
			GregorianCalendar calendar( CalendarInfoSourceType::SYSTEM );

			const auto& d = calendar.GetDate();
			const auto daysMatch = d.GetDay() == systemTime.wDay;
			const auto dotwMatch = d.GetDay().GetDayOfTheWeek() == systemTime.wDayOfWeek;
			const auto monthMatch = d.GetMonth() == systemTime.wMonth;
			const auto yearMatch = systemTime.wYear == d.GetYear();

			VERIFY( daysMatch );
			VERIFY( dotwMatch );
			VERIFY( monthMatch );
			VERIFY( yearMatch );

			const auto& t = calendar.GetTime();
			const auto hourMatch = t.GetHour() == systemTime.wHour;
			const auto minMatch = t.GetMinute() == systemTime.wMinute;
			const auto secMatch = t.GetSecond() == systemTime.wSecond;
			const auto milliMatch = t.GetMillisecond() == systemTime.wMilliseconds;

			VERIFY( hourMatch );
			VERIFY( minMatch );
			VERIFY( secMatch );
			VERIFY( milliMatch );
		}
	}
}
#endif
