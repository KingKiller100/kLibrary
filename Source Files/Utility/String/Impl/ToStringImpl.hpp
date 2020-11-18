#pragma once

#include <string>

#include "../../../TypeTraits/StringTraits.hpp"
#include "../kStringManipulation.hpp"

namespace klib::kString::impl
{
	template<class Char_t>
	void ToStringImpl(const std::basic_string<Char_t>& fmt, std::basic_string<Char_t>& finalString, size_t currentIndex)
	{}


	template<class Char_t, typename T, typename ...Ts>
	void ToStringImpl(std::basic_string<Char_t>& fmt, std::basic_string<Char_t>& finalString, size_t textPos
		, T arg, Ts ...args)
	{
		constexpr Char_t printfSymbol = Char_t('%');
		constexpr Char_t openerSymbol = Char_t('{');
		constexpr Char_t closerSymbol = Char_t('}');
		constexpr Char_t specifierSymbol = Char_t(':');
		constexpr Char_t nullTerminator = type_trait::s_NullTerminator<Char_t>;
		constexpr size_t npos = std::basic_string_view<Char_t>::npos;

		auto openerPos = fmt.find_first_of(openerSymbol, textPos);
		auto closerPos = fmt.find_first_of(closerSymbol, openerSymbol);
		if (openerPos == npos
			|| closerPos == npos)
			return;

		while (openerPos != npos)
		{
			const auto infoSize = closerPos - textPos;
			//std::basic_string<Char_t> currentSection = fmt.substr(textPos, infoSize);

			const auto replacePos = openerPos - textPos;
			const auto colonPos = fmt.find_first_of(specifierSymbol, replacePos);
			const auto objIndexSize = colonPos - openerPos;

			const auto objIndex = StrTo<long long>(fmt.substr(openerPos, objIndexSize));

			std::basic_string<Char_t> specifier;
			if (colonPos != npos)
			{
				const auto startPos = colonPos + 1;
				const auto count = (fmt.find_first_of(clos) - 1) - startPos;
				specifier = currentSection.substr(startPos, count);
			}

			//currentSection.erase(replacePos);
			const auto replacement = Stringify<Char_t, T>(arg, specifier);

		}

		if (fmt.size() - 1 >= prevCloserIndex)
			finalString.append(fmt.substr(prevCloserIndex));

	}
}
