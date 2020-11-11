#include "pch.hpp"
#include "kComponentToStringImpl.hpp"

#include "../../../Type Traits/StringTraits.hpp"

#include "../../String/kToString.hpp"

namespace klib::kCalendar
{
	constexpr auto noMatchToken = type_trait::s_NullTerminator<char>;

	void CalendarComponentToStringImpl::ToStringImpl(const std::string_view& format, const std::set<char>& tokens
		, const std::function<void(char)>& noMatchingTokenFunc
		, const std::function<void(size_t, char)>& matchingTokenFunc) const
	{
		size_t index = 0;

		for (auto letter = format.front();
			letter != type_trait::s_NullTerminator<char>;
			letter = format[++index])
		{
			auto match(noMatchToken);
			for (const auto& token : tokens)
			{
				if (token == letter)
					match = token;
			}

			if (match == noMatchToken)
			{
				noMatchingTokenFunc(letter);
				continue;
			}

			letter = kString::ToLower(letter);
			
			const auto first = format.find_first_of(match, index);
			auto last = format.find_first_not_of(match, first);

			if (last == std::string::npos)
				last = format.size();

			const auto count = last - first;

			matchingTokenFunc(count, letter);

			index += count - 1;

			if (index >= format.size() - 1)
				break;
		}
	}

}
