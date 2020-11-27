#pragma once

#include "../FormatSymbols.hpp"
#include "../kStringIdentity.hpp"
#include "../../Tricks/kStringData.hpp"
#include "../../../../TypeTraits/StringTraits.hpp"
#include "../../../../TypeTraits/ConditionalReference.hpp"
#include <string>


namespace klib::kString::secret::impl
{
	template<class Char_t>
	constexpr void ToStringImpl(UNUSED const std::basic_string<Char_t>& fmt, UNUSED const size_t argIndex)
	{}

	template<class Char_t>
	USE_RESULT constexpr size_t FindOpenerPosition(std::basic_string<Char_t>& outFmt, const size_t argIndex)
	{
		using namespace stringify;
		using Str_t = std::basic_string<Char_t>;
		constexpr size_t npos = type_trait::g_NoPos<Str_t>;

		Char_t buff[5]{ type_trait::g_NullTerminator<Char_t> }; // '{' 'x' 'x' '}/:' '\0'
		Char_t* const end = std::end(buff) - 1;
		Char_t* current = end;

		*(--current) = format::g_CloserSymbol<Char_t>;
		current = stringify::UintToStr(current, argIndex);
		*(--current) = format::g_OpenerSymbol<Char_t>;
		
		size_t openerPos = Find(outFmt.data(), current);
		if (openerPos == npos)
		{
			*(end - 1) = format::g_SpecifierSymbol<Char_t>;
			openerPos = Find(outFmt.data(), current);
		}

		return openerPos;
	}

	template<class Char_t, typename T, typename ...Ts>
	constexpr void ToStringImpl(std::basic_string<Char_t>& outFmt, const size_t argIndex, const T& arg, const Ts& ...argPack)
	{
		using namespace stringify;
		using Str_t = std::basic_string<Char_t>;
		constexpr auto npos = type_trait::g_NoPos<Str_t>;

		size_t openerPos = FindOpenerPosition(outFmt, argIndex);
		size_t closerPos = Find_First_Of(outFmt.data(), format::g_CloserSymbol<Char_t>, openerPos);

		if (openerPos == npos || closerPos == npos)
			return;

		while (openerPos != npos && closerPos != npos)
		{
			const auto infoSize = closerPos - openerPos;
			const size_t colonPos = Find_First_Of(outFmt.data(), format::g_SpecifierSymbol<Char_t>, openerPos);

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
			outFmt.insert(openerPos, GetData(replacement), GetSize( replacement ));

			if _CONSTEXPR_IF(std::is_pointer_v<decltype(replacement)>)
			{
				delete[] replacement;
			}
			
			openerPos = FindOpenerPosition(outFmt, argIndex);
			closerPos = Find_First_Of(outFmt.data(), format::g_CloserSymbol<Char_t>, openerPos);
		}

		ToStringImpl<Char_t, Ts...>(outFmt, argIndex + 1, argPack...);
	}
}
