#include "pch.hpp"
#include "kMonth.hpp"
#include "kYear.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Month::ToString(const std::string_view& format) const
	{
		std::string output;

		const auto noMatchFunc = [&](char noToken)
		{
			output.push_back(noToken);
		};

		const auto matchFunc = [&](size_t count, char)
		{
			output.append(ToStringUsingTokenCount(count));
		};

		ToStringImpl(format, { FormatToken }, noMatchFunc, matchFunc);
		return output;
	}

	std::string Month::ToStringUsingTokenCount(const size_t count) const
	{
		if (count >= 4)
			return kString::ToString<char>(GetMonthStr());
		if (count == 3)
			return  kString::ToString<char>(GetMonthStr().substr(0, 3));
		if (count == 2)
			return kString::ToString("{0:2}", GetMonthNumber());

		return  kString::ToString<char>(GetMonthNumber());
	}

	bool Month::Verify(const Day& day, const Year& year) const
	{
		switch (moty)
		{
		case FEB: return year.IsLeapYear()
			? day.GetValue() <= 29
			: day.GetValue() <= 28;

		case JAN:
		case MAR:
		case MAY:
		case JUL:
		case AUG:
		case OCT:
		case DEC: return day.GetValue() <= 31;

		case APR:
		case JUN:
		case SEP:
		case NOV: return day.GetValue() <= 30;

		default: return false;
		}
	}

	std::string Month::GetMonthStr() const
	{
		return MonthToString(moty);
	}
}
