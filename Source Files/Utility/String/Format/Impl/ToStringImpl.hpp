#pragma once

#include "Stringify.hpp"
#include "../FormatSymbols.hpp"
#include "../../kStringTricks.hpp"
#include "../../../../TypeTraits/StringTraits.hpp"
#include <string>


namespace klib::kString::stringify
{
	template<class Char_t>
	constexpr void ToStringImpl(UNUSED const std::basic_string<Char_t>& fmt, UNUSED size_t currentIndex)
	{}

	template<class Char_t, typename T, typename ...Ts>
	constexpr void ToStringImpl(std::basic_string<Char_t>& outFmt, size_t textPos, T arg, Ts ...argPack)
	{
		constexpr auto npos = type_trait::npos<std::basic_string<Char_t>>;
		
		size_t openerPos = outFmt.find_first_of(format::openerSymbol<Char_t>, textPos);
		size_t closerPos = outFmt.find_first_of(format::closerSymbol<Char_t>, openerPos);

		if (openerPos == npos
			|| closerPos == npos)
			return;

		if (outFmt[openerPos + 1] == format::openerSymbol<Char_t> // Escape
			|| outFmt[openerPos + 1] == Char_t(' ')
			|| outFmt[openerPos + 1] == Char_t('\t')
			|| outFmt[openerPos + 1] == Char_t('\n')
			|| outFmt[openerPos + 1] == Char_t('\r')
			|| outFmt[openerPos + 1] == type_trait::s_NullTerminator<Char_t>)
		{
			ToStringImpl<Char_t, T, Ts...>(outFmt, openerPos + 2, arg, argPack...);
		}

		const size_t origOpenerPos = openerPos;

		const auto infoSize = closerPos - openerPos;
		StringWriter<Char_t> objIndexStr;
		StringWriter<Char_t> specifier;

		const auto replacePos = openerPos - textPos;
		const size_t colonPos = outFmt.find_first_of(format::specifierSymbol<Char_t>, replacePos);

		if (openerPos < colonPos
			&& closerPos > colonPos)
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

		size_t spacesToSkip = 0;
		bool skipSet = false;
		while (openerPos != npos && closerPos != npos)
		{
			const auto replacement = Stringify<Char_t, T>(arg, specifier);
			if (!skipSet)
			{
				spacesToSkip = GetSize(replacement);
				skipSet = true;
			}
			outFmt.erase(openerPos, infoSize + 1);
			outFmt.insert(openerPos, replacement);
			openerPos = outFmt.find(format::openerSymbol<Char_t> + objIndexStr);
			closerPos = outFmt.find_first_of(format::closerSymbol<Char_t>, openerPos);
		}

		ToStringImpl<Char_t, Ts...>(outFmt, origOpenerPos + spacesToSkip, argPack...);
	}
}
