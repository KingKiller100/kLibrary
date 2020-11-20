#pragma once

#include "Stringify.hpp"
#include "../FormatSymbols.hpp"
#include "../../kStringTricks.hpp"
#include "../../../../TypeTraits/StringTraits.hpp"
#include <string>


namespace klib::kString::stringify
{
	template<class Char_t>
	constexpr void ToStringImpl(UNUSED const std::basic_string<Char_t>& fmt, const size_t argIndex, UNUSED size_t currentIndex)
	{}

	template<class Char_t, typename T, typename ...Ts>
	constexpr void ToStringImpl(std::basic_string<Char_t>& outFmt, size_t textPos, const size_t argIndex, T arg, Ts ...argPack)
	{
		constexpr auto npos = type_trait::s_NoPos<std::basic_string<Char_t>>;

		size_t openerPos = outFmt.find_first_of(format::s_OpenerSymbol<Char_t>, textPos);
		size_t closerPos = outFmt.find_first_of(format::s_CloserSymbol<Char_t>, openerPos);

		if (openerPos == npos
			|| closerPos == npos)
			return;

		if (outFmt[openerPos + 1] == format::s_OpenerSymbol<Char_t> // Escape
			|| IsWhiteSpace(outFmt[openerPos + 1])
			|| outFmt[openerPos + 1] == type_trait::s_NullTerminator<Char_t>)
		{
			ToStringImpl<Char_t, T, Ts...>(outFmt, openerPos + 2, argIndex, arg, argPack...);
		}

		const size_t origOpenerPos = openerPos;

		const auto infoSize = closerPos - openerPos;
		StringWriter<Char_t> objIndexStr;
		StringWriter<Char_t> specifier;

		const auto replacePos = openerPos - textPos;
		const size_t colonPos = outFmt.find_first_of(format::s_SpecifierSymbol<Char_t>, openerPos);

		if (openerPos < colonPos
			&& closerPos > colonPos)
		{
			const auto objIndexSize = colonPos - (openerPos + 1);
			objIndexStr = outFmt.substr(openerPos + 1, objIndexSize);

			const auto startPos = colonPos + 1;
			const auto count = closerPos - startPos;
			specifier = outFmt.substr(startPos, count);
		}
		else
		{
			objIndexStr = outFmt.substr(openerPos + 1, infoSize - 1);
		}

		const auto scopeArgIndex = StrTo<long long>(objIndexStr);

		/// If the object is meant to be emplaced at a different point of the string
		/// We neeed to call this funciton again looking for the correct part of the
		/// string to add this same object.
		if (scopeArgIndex != argIndex)
		{
			ToStringImpl<Char_t, Ts...>(outFmt, textPos, argIndex + 1, argPack...);
			ToStringImpl<Char_t, T, Ts...>(outFmt, textPos, argIndex, arg, argPack...);
			return;
		}

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
			openerPos = outFmt.find(format::s_OpenerSymbol<Char_t> +objIndexStr);
			closerPos = outFmt.find_first_of(format::s_CloserSymbol<Char_t>, openerPos);
		}

		ToStringImpl<Char_t, Ts...>(outFmt, origOpenerPos + spacesToSkip, argIndex + 1, argPack...);
	}
}
