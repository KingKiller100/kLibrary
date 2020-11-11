#include "pch.hpp"
#include "kYear.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	std::string Year::GetYearStr() const
	{
		return kString::ToString<char>("{0:4}", year);
	}

	std::string Year::ToString(const std::string_view& format) const
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

	std::string Year::ToStringUsingTokenCount(const size_t count) const
	{
		if (count <= 2)
			return GetYearStr().substr(2);

		return GetYearStr();
	}
}
