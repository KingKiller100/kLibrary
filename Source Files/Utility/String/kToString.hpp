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
		USE_RESULT constexpr std::basic_string<CharT> ToString(const CharT* format, const T& arg, const Ts& ...argPack)
		{
			if (Find_First_Of(format, format::g_PrintfSymbol<CharT>) != type_trait::g_NoPos<std::basic_string<CharT>>)
			{
				return SprintfWrapper<CharT>(format, arg, argPack...);
			}

			auto finalString = ToWriter(format);
			secret::impl::ToStringImpl<CharT, T, Ts...>(finalString, 0, 0, arg, argPack...);
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
			constexpr auto count = std::variant_size_v<DataTypes> - static_cast<size_t>(1);

			size_t reserveSize = 0;

			if _CONSTEXPR_IF(count < 10)
				reserveSize = count + 2 * count;
			else
				reserveSize = 27 + (count - 9) * 4;

			std::basic_string<CharT> output;
			output.reserve(reserveSize);
			for (size_t i = 0; i < count; ++i)
			{
				output.push_back(format::g_OpenerSymbol<CharT>);
				output.append(stringify::StringUnsignedIntegral<CharT>(i));
				output.push_back(format::g_CloserSymbol<CharT>);
			}

			secret::impl::ToStringImpl<CharT, T, Ts...>(output, 0, 0, arg, argPack...);
			return output;
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif
}
