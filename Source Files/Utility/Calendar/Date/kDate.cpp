#include <pch.hpp>

#include "kDate.hpp"

#include "../Secret/kiCalendarInfoSource.hpp"

#include "../../String/kToString.hpp"
#include "../../Debug Helper/Exceptions/CalenderExceptions.hpp"

#include "../../../Type Traits/StringTraits.hpp"

#include <set>


namespace klib::kCalendar
{
	namespace
	{
		using namespace secret::helper;

		iCalendarInfoSource& GetInfoSource(CalendarInfoSourceType type)
		{
			static iCalendarInfoSource& calendar_info = GetCalendarInfoSource();
			calendar_info.Refresh(type);
			return calendar_info;
		}
	}

	// DATE ////////////////////////////////////////////////////////////

	Date::Date(CalendarInfoSourceType sourceType)
		: Date(GetInfoSource(sourceType))
	{}

	Date::Date(const iCalendarInfoSource& source)
		: Date(
			static_cast<Day::DayOfTheWeek>(source.GetDayOfTheWeekIndex())
			, source.GetDay()
			, static_cast<Month::MonthOfTheYear>(source.GetMonth())
			, source.GetYear()
		)
	{}

	Date::Date(const Day::DayOfTheWeek dayOfTheWeek, const std::uint16_t d, const Month::MonthOfTheYear m,
		const std::uint16_t y)
		: day(d, dayOfTheWeek)
		, month(m)
		, year(y)
	{
		CheckDate();
	}

	void Date::CheckDate() const
	{
		if (!day.Verify())
			throw kDebug::CalendarError();

		if (!month.Verify(day, year))
			throw kDebug::InvalidMonthError();
	}

	const Day& Date::GetDay() const
	{
		return day;
	}

	const Month& Date::GetMonth() const
	{
		return month;
	}

	const Year& Date::GetYear() const
	{
		return year;
	}

	Day Date::GetDay()
	{
		return day;
	}

	Month& Date::GetMonth()
	{
		return month;
	}

	Year& Date::GetYear()
	{
		return year;
	}

	std::string Date::ToString(const std::string_view& format) const
	{
		constexpr auto noMatchToken = type_trait::s_NullTerminator<char>;
		
		const std::set<char> tokens{ Day::FormatToken, Month::FormatToken, Year::FormatToken };

		std::string output;

		const auto noMatchFunc = [&](char noToken)
		{
			output.push_back(noToken);
		};

		const auto matchFunc = [&](size_t count, char token)
		{
			switch (kString::ToLower(token))
			{
			case Day::FormatToken:
				output.append(day.ToStringUsingTokenCount(count));
				break;

			case Month::FormatToken:
				output.append(month.ToStringUsingTokenCount(count));
				break;

			case Year::FormatToken:
				output.append(year.ToStringUsingTokenCount(count));
				break;

			default:
				throw kDebug::CalendarError("Bad format for date");
			}
		};

		ToStringImpl(format, tokens, noMatchFunc, matchFunc);
		return output;
	}

	std::string Date::ToString(DateNumericalSeparator separator) const
	{
		std::string separatorStr;
		separatorStr += (separator);

		std::string str(
			kString::ToString("{0:2}{1}{2:2}{1}{3:4}"
				, static_cast<std::uint16_t>(GetDay())
				, separatorStr
				, static_cast<std::uint16_t>(GetMonth())
				, static_cast<std::uint16_t>(GetYear())));

		return str;
	}

	std::string Date::ToString(DateTextLength textLength) const
	{
		const auto dayStr = day.GetDayStr();
		const auto monthStr = month.GetMonthStr();
		const auto yearStr = year.GetYearStr();

		std::string str;

		switch (textLength)
		{
		case DateTextLength::FULL:
		{
			const auto d_o_t_wStr = day.GetDayOfTheWeekStr();
			str = kString::ToString("{0} {1} {2} {3}"
				, d_o_t_wStr
				, dayStr
				, monthStr
				, yearStr);
		}
		break;

		case DateTextLength::SHORT:
			str = kString::ToString("{0} {1} {2}"
				, monthStr
				, dayStr
				, yearStr
			);
			break;

		default:
			throw kDebug::CalendarError("Unknown date text format");
		}

		return str;
	}

}

