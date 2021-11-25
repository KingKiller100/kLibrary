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
	USE_RESULT constexpr size_t FindOpenerPosition(const std::basic_string<Char_t>& outFmt, const size_t argIndex, const size_t offset)
	{
		using namespace stringify;
		using Str_t = std::basic_string<Char_t>;
		constexpr size_t npos = type_trait::g_NoPos<Str_t>;

		const auto data = outFmt.data();
		
		Char_t buff[5]{ Char_t(), Char_t(), Char_t(), format::g_ToStringCloserSymbol<Char_t> }; // '{' 'x' 'x' '}/:' '\0'
		Char_t* const pCloser = std::end(buff) - 2;
		Char_t* searchStr = pCloser;

		searchStr = stringify::UintToStr(searchStr, argIndex, 10);
		*(--searchStr) = format::g_ToStringOpenerSymbol<Char_t>;

		size_t openerPos = Find(data, searchStr, offset);
		if (openerPos != npos)
			return openerPos;

		openerPos = Find(data, searchStr, 0);
		if (openerPos != npos)
			return openerPos;

		*pCloser = format::g_ToStringSpecifierSymbol<Char_t>;
		openerPos = Find(data, searchStr, offset);

		if (openerPos != npos)
			return openerPos;

		openerPos = Find(data, searchStr, 0);
		return openerPos;
	}

	template<class Char_t>
	constexpr void ToStringImpl(const std::basic_string<Char_t>&, const size_t, const size_t)
	{}

	template<class Char_t, typename T, typename ...Ts>
	constexpr void ToStringImpl(std::basic_string<Char_t>& outFmt, const size_t argIndex, const size_t offset, type_trait::SizeCondConstRef_t<T> arg, type_trait::SizeCondConstRef_t<Ts> ...argPack)
	{
		using namespace stringify;
		using Str_t = std::basic_string<Char_t>;
		constexpr auto npos = type_trait::g_NoPos<Str_t>;

		try
		{
			size_t openerPos = FindOpenerPosition(outFmt, argIndex, offset);
			size_t closerPos = Find_First_Of(GetData(outFmt), format::g_ToStringCloserSymbol<Char_t>, openerPos);

			const auto initialOpenerPos = openerPos;
			size_t repSize = 0;

			while (openerPos != npos && closerPos != npos)
			{
				const auto infoSize = closerPos - openerPos;
				const size_t colonPos = Find_First_Of(GetData(outFmt), format::g_ToStringSpecifierSymbol<Char_t>, openerPos);

				StringWriter<Char_t> specifier;

				if (openerPos < colonPos
					&& closerPos > colonPos)
				{
					const auto startPos = colonPos + 1;
					const auto count = closerPos - startPos;
					specifier = outFmt.substr(startPos, count);
				}

				const auto replacement = Identity<Char_t, T>::MakeStr(arg, specifier);
				repSize = GetSize(replacement);
				outFmt.erase(openerPos, infoSize + 1);
				outFmt.insert(openerPos, GetData(replacement), repSize);

				if constexpr(type_trait::Is_CString_V<decltype(replacement)>)
				{
					delete[] replacement;
				}

				openerPos = FindOpenerPosition(outFmt, argIndex, openerPos + repSize);
				if (openerPos != npos)
					closerPos = Find_First_Of(GetData(outFmt), format::g_ToStringCloserSymbol<Char_t>, openerPos);
			}

			ToStringImpl<Char_t, Ts...>(outFmt, argIndex + 1, initialOpenerPos + repSize, argPack...);
		}
		catch (const std::exception& e)
		{
			throw kDebug::FormatError(e.what());
		}
	}
}
