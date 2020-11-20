#pragma once

#include "Stringify.hpp"
#include "../FormatSymbols.hpp"
#include "../../kStringTricks.hpp"
#include "../../../../TypeTraits/StringTraits.hpp"
#include <string>


namespace klib::kString::stringify
{
	template<class Char_t>
	constexpr void ToStringImpl(UNUSED const std::basic_string<Char_t>& fmt, UNUSED const size_t argIndex)
	{}

	template<class Char_t, typename T, typename ...Ts>
	constexpr void ToStringImpl(std::basic_string<Char_t>& outFmt, const size_t argIndex, T arg, Ts ...argPack)
	{
		using Str_t = std::basic_string<Char_t>;
		constexpr auto npos = type_trait::s_NoPos<Str_t>;

		const auto searchStr = format::s_OpenerSymbol<Char_t> + stringify::StringIntegral<Char_t>(argIndex);
		
		size_t openerPos = outFmt.find_first_of(searchStr);
		size_t closerPos = outFmt.find_first_of(format::s_CloserSymbol<Char_t>, openerPos);

		if (openerPos == npos || closerPos == npos)
			return;

		while (openerPos != npos && closerPos != npos)
		{
			const auto infoSize = closerPos - openerPos;
			const size_t colonPos = outFmt.find_first_of(format::s_SpecifierSymbol<Char_t>, openerPos);

			StringWriter<Char_t> specifier;

			if (openerPos < colonPos
				&& closerPos > colonPos)
			{
				const auto startPos = colonPos + 1;
				const auto count = closerPos - startPos;
				specifier = outFmt.substr(startPos, count);
			}

			const auto replacement = Stringify<Char_t, T>(arg, specifier);
			outFmt.erase(openerPos, infoSize + 1);
			outFmt.insert(openerPos, replacement);
			openerPos = outFmt.find(searchStr);
			closerPos = outFmt.find_first_of(format::s_CloserSymbol<Char_t>, openerPos);
		}

		ToStringImpl<Char_t, Ts...>(outFmt, argIndex + 1, argPack...);
	}
}
