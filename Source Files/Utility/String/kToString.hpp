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
		namespace secret::impl
		{
			/**
			 * \brief
			 *		Estimates the initial size of the dynamically created format string for ToString uses
			 *		Work around function needed to compile on v141 toolset without any compiler warnings
			 * \tparam Count
			 *		Number of arguments
			 * \return
			 *	Number of characters needed for the format string
			 */
			template<size_t Count>
			constexpr size_t InitialFormatStringSize()
			{
				if _CONSTEXPR_IF(Count < 10)
				{
					return Count + 2 * Count;
				}
				else
				{
					return 27 + (Count - 9) * 4;
				}
			}
		}
		
		// Outputs a interpolated string with data given for all string types. NOTE: Best performance with char and wchar_t type strings
		template<class CharT, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharT> ToString(const CharT* format, const T& arg, const Ts& ...argPack)
		{
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

		struct NoFormatTag { NoFormatTag() = default; };

		template<class CharT, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharT> ToString(NoFormatTag&&, const T& arg, const  Ts& ...argPack)
		{
			using DataTypes = std::variant<std::monostate, T, Ts...>;
			constexpr auto argCount = std::variant_size_v<DataTypes> -1;
			constexpr size_t reserveSize = secret::impl::InitialFormatStringSize<argCount>();

			std::basic_string<CharT> output;
			output.reserve(reserveSize);
			for (size_t i = 0; i < argCount; ++i)
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
