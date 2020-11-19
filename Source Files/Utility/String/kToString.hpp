#pragma once

#include "Impl/ToStringImpl.hpp"

#include "Stringify/kSprintf.hpp"
#include "Stringify/kFormatMarkers.hpp"
#include "Stringify/kStringIdentity.hpp"
#include "Stringify/kStringifyBool.hpp"
#include "Stringify/kStringifyFloatingPoint.hpp"
#include "Stringify/kStringifyInteger.hpp"
#include "Stringify/kStringifyPointer.hpp"


#include "../Debug/Exceptions/StringExceptions.hpp"

#include "../../TypeTraits/StringTraits.hpp"
#include "../../Utility/String/kStringConverter.hpp"

#include <any>
#include <array>
#include <string>
#include <variant>


namespace klib {
	namespace kString
	{
		// Outputs a interpolated string with data given for all string types. NOTE: Best performance with char and wchar_t type strings
		template<class CharType, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharType> ToString(const std::basic_string_view<CharType>& format, T arg, Ts ...argPack)
		{
			using namespace secret::helper;
			using DataTypes = std::variant<std::monostate, T, Ts...>;

			constexpr auto printfSymbol = CharType('%');
			constexpr auto openerSymbol = CharType('{');
			constexpr auto closerSymbol = CharType('}');
			constexpr auto specifierSymbol = CharType(':');
			constexpr auto nullTerminator = type_trait::s_NullTerminator<CharType>;
			constexpr auto npos = std::basic_string_view<CharType>::npos;

			if (format.find(printfSymbol) != npos)
			{
				return stringify::SprintfWrapper<CharType>(format, arg, argPack...);
			}

			std::basic_string<CharType> finalString(format);
			impl::ToStringImpl<CharType, T, Ts...>(finalString, 0, arg, argPack...);
			return finalString;
		}

		template<class CharType, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharType> ToString(const std::basic_string<CharType>& format, const T& arg, const Ts& ...argPack)
		{
			const std::basic_string_view<CharType> fmt(format);
			const std::basic_string<CharType> text = ToString(fmt, arg, argPack...);
			return text;
		}

		template<class CharType, typename T, typename ...Ts>
		USE_RESULT constexpr std::basic_string<CharType> ToString(const CharType* format, const T& arg, const Ts& ...argPack)
		{
			const std::basic_string_view<CharType> fmt(format);
			const std::basic_string<CharType> text = ToString(fmt, arg, argPack...);
			return text;
		}

		template<class CharType, typename T, typename ...Ts>
		constexpr std::basic_string<CharType> ToString(const T& arg, const  Ts& ...argPack)
		{
			using DataTypes = std::variant<std::monostate, T, Ts...>;
			constexpr auto count = std::variant_size_v<DataTypes> -1;
			std::basic_string<CharType> format;

			for (auto i = 0; i < count; ++i)
			{
				format.push_back(CharType('{'));
				format.append(stringify::StringIntegral<CharType>(i, 1, CharType('0')));
				format.push_back(CharType('}'));
			}

			const auto output = ToString(format, arg, argPack...);
			return output;
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif
}
