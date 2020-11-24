#pragma once

#include "kSprintf.hpp"
#include "Format/FormatSymbols.hpp"
#include "Format/Impl/ToStringImpl.hpp"

#include "kStringTricks.hpp"

#include <string>
#include <variant>

namespace klib {
	namespace kString
	{		
		// Outputs a interpolated string with data given for all string types. NOTE: Best performance with char and wchar_t type strings
		template<class CharT, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharT> ToString(const CharT* format, T arg, Ts ...argPack)
		{
			if (Find_First_Of(format, format::s_PrintfSymbol<CharT>) != type_trait::s_NoPos<std::basic_string<CharT>>)
			{
				return SprintfWrapper<CharT>(format, arg, argPack...);
			}

			auto finalString = ToWriter(format);
			secret::impl::ToStringImpl<CharT, T, Ts...>(finalString, 0, arg, argPack...);
			return finalString;
		}

		template<class StringT, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<typename StringT::value_type> ToString(const StringT& format, const T& arg, const Ts& ...argPack)
		{
			const auto text = ToString(format.data(), arg, argPack...);
			return text;
		}

		template<class CharT, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharT> ToString(const T& arg, const  Ts& ...argPack)
		{
			using DataTypes = std::variant<std::monostate, T, Ts...>;
			constexpr auto count = std::variant_size_v<DataTypes> -1;
			
			std::basic_string<CharT> format;
			format.reserve(count + 2 * count);
			for (auto i = 0; i < count; ++i)
			{
				format.push_back(format::s_OpenerSymbol<CharT>);
				format.append(stringify::StringIntegral<CharT>(i, 1));
				format.push_back(format::s_CloserSymbol<CharT>);
			}

			const auto output = ToString(format.data(), arg, argPack...);
			return output;
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif
}
