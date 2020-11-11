#include "pch.hpp"
#include "kMinute.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Minute::ToString(const std::string_view& format) const
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

	std::string Minute::ToStringUsingTokenCount(const size_t count) const
	{
		const auto numberFormat = "{0:" + kString::ToString<char>(count) + "}";

		const auto minStr = (count < 3)
			? kString::ToString(numberFormat, GetValue())
			: (count == 3)
			? kString::ToString("{0}{1}", GetValue(), Units)
			: kString::ToString("{0:2}{1}", GetValue(), Units);
		
		return minStr;
	}
}
