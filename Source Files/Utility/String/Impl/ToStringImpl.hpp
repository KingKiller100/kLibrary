#pragma once


#include "Stringify.hpp"
#include "../kStringManipulation.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include <string>

namespace klib::kString::impl
{
	template<class Char_t>
	void ToStringImpl(const std::basic_string<Char_t>& fmt, size_t currentIndex)
	{}

	template<class Char_t, typename T, typename ...Ts>
	void ToStringImpl(std::basic_string<Char_t>& outFmt, size_t textPos, T arg, Ts ...argPack)
	{
		constexpr Char_t printfSymbol = Char_t('%');
		constexpr Char_t openerSymbol = Char_t('{');
		constexpr Char_t closerSymbol = Char_t('}');
		constexpr Char_t specifierSymbol = Char_t(':');
		constexpr Char_t nullTerminator = type_trait::s_NullTerminator<Char_t>;
		constexpr size_t npos = std::basic_string<Char_t>::npos;

		size_t openerPos = outFmt.find_first_of(openerSymbol, textPos);
		size_t closerPos = outFmt.find_first_of(closerSymbol, openerPos);
		
		if (openerPos == npos
			|| closerPos == npos)
			return;

		if (outFmt[openerPos + 1] == openerSymbol // Escape
			|| outFmt[openerPos + 1] == Char_t(' ')
			|| outFmt[openerPos + 1] == Char_t('\t')
			|| outFmt[openerPos + 1] == Char_t('\n')
			|| outFmt[openerPos + 1] == Char_t('\r')
			|| outFmt[openerPos + 1] == nullTerminator)
		{
			ToStringImpl<Char_t, Ts...>(outFmt, openerPos + 2, argPack...);
		}

		const auto infoSize = closerPos - openerPos;
		StringWriter<Char_t> objIndexStr;
		StringWriter<Char_t> specifier;

		const auto replacePos = openerPos - textPos;
		const size_t colonPos = outFmt.find_first_of(specifierSymbol, replacePos);

		if (colonPos != npos)
		{
			const auto objIndexSize = (colonPos - 1) - (openerPos + 1);
			objIndexStr = outFmt.substr(openerPos, objIndexSize);

			const auto startPos = colonPos + 1;
			const auto count = (closerPos - 1) - startPos;
			specifier = outFmt.substr(startPos, count);
		}
		else
		{
			objIndexStr = outFmt.substr(openerPos + 1, infoSize - 1);
		}
		
		const auto objIndex = StrTo<long long>(objIndexStr);

		while (outFmt.find(openerSymbol + objIndexStr) != npos)
		{
			const auto replacement = Stringify<Char_t, T>(arg, specifier);
			outFmt.erase(openerPos, infoSize + 1);
			outFmt.insert(openerPos, replacement);
		}

		ToStringImpl<Char_t, Ts...>(outFmt, closerPos + 1, argPack...);
	}
}
