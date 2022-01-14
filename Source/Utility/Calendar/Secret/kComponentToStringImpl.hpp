#pragma once

#include "../../../HelperMacros.hpp"

#include <functional>
#include <set>

namespace klib::kCalendar
{
	class CalendarComponentToStringImpl
	{
	protected:
		CalendarComponentToStringImpl() = default;

		void ToStringImpl(
			const std::string_view& format
			, const std::set<char>& tokens
			, const std::function<void( char )>& noMatchingTokenFunc
			, const std::function<void( size_t, char )>& matchingTokenFunc
		) const;
	};
}
