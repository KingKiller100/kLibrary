#pragma once

#include "../FormatSymbols.hpp"
#include "../kStringIdentity.hpp"
#include "../../../../TypeTraits/StringTraits.hpp"
#include <string>


namespace klib::kString::secret::impl
{
	template<class Char_t>
	constexpr void ToStringImpl(UNUSED const std::basic_string<Char_t>& fmt, UNUSED const size_t argIndex)
	{}

	template<class Char_t>
	USE_RESULT constexpr size_t FindOpenerPos(std::basic_string<Char_t>& outFmt, const size_t argIndex)
	{
		using namespace stringify;
		using Str_t = std::basic_string<Char_t>;
		constexpr size_t npos = type_trait::s_NoPos<Str_t>;
		
		auto searchStr = format::s_OpenerSymbol<Char_t> + StringIntegral<Char_t>(argIndex) + format::s_CloserSymbol<Char_t>;
		size_t openerPos = outFmt.find(searchStr);
		if (openerPos == npos)
		{
			searchStr = format::s_OpenerSymbol<Char_t> +StringIntegral<Char_t>(argIndex) + format::s_SpecifierSymbol<Char_t>;
			openerPos = outFmt.find(searchStr);
		}

		return openerPos;
	}

	template<class Char_t, typename T, typename ...Ts>
	constexpr void ToStringImpl(std::basic_string<Char_t>& outFmt, const size_t argIndex, T arg, Ts ...argPack)
	{
		using namespace stringify;
		using Str_t = std::basic_string<Char_t>;
		constexpr auto npos = type_trait::s_NoPos<Str_t>;

		size_t openerPos = FindOpenerPos(outFmt, argIndex);
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

			const auto replacement = Identity<Char_t, T>::MakeStr(arg, specifier);
			outFmt.erase(openerPos, infoSize + 1);
			outFmt.insert(openerPos, replacement);
			openerPos = FindOpenerPos(outFmt, argIndex);
			closerPos = outFmt.find_first_of(format::s_CloserSymbol<Char_t>, openerPos);
		}

		ToStringImpl<Char_t, Ts...>(outFmt, argIndex + 1, argPack...);
	}
}
