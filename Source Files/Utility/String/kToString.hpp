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
		// template<typename CharType, size_t Size, typename ...Ts>
		// secret::helper::FormatMarkerQueue CreateIdentifiers(const std::basic_string<CharType>& fmt, secret::helper::FormatMarkerQueue& identifiers
		// 	, size_t currentIndex = 0)
		// {
		// 	using namespace secret::helper;
		//
		// 	static constexpr auto openerSymbol = CharType('{');
		// 	static constexpr auto closerSymbol = CharType('}');
		// 	static constexpr auto specifierSymbol = CharType(':');
		// 	static constexpr auto nullTerminator = type_trait::s_NullTerminator<CharType>;
		// 	static constexpr auto npos = std::basic_string<CharType>::npos;
		//
		// 	FormatMarker::PositionType openerPos = fmt.find_first_of(openerSymbol, currentIndex);
		//
		// 	if (openerPos == npos)
		// 		return identifiers;
		// 	
		// 	if (fmt[openerPos + 1] == openerSymbol ||
		// 		fmt[openerPos + 1] == CharType(' ') ||
		// 		fmt[openerPos + 1] == CharType('\t') ||
		// 		fmt[openerPos + 1] == CharType('\n') ||
		// 		fmt[openerPos + 1] == CharType('\r') ||
		// 		fmt[openerPos + 1] == nullTerminator)
		// 	{
		// 		openerPos += 2;
		// 		return CreateIdentifiers<CharType, Size, T, Ts...>(fmt, identifiers, currentIndex);
		// 	}
		//
		// 	const auto closePos = fmt.find_first_of(closerSymbol, openerPos);
		// 	const auto digits = closePos - openerPos - 1;
		// 	const auto current = fmt.substr(openerPos + 1, digits);
		// 	std::string bracketContents = kString::Convert<char>(current);
		//
		// 	const auto relativeColonPos = bracketContents.find_first_of(specifierSymbol);
		// 	const auto optionIndex = bracketContents.substr(0, relativeColonPos);
		// 	const auto idx = kString::StrTo<FormatMarker::IndexType>(optionIndex);
		// 	const FormatMarker::NameType type = typeid(Ts).name();
		//
		// 	if (Size <= idx)
		// 	{
		// 		const auto convertedFmt = kString::Convert<char>(fmt);
		// 		const auto errMsg = "Index given is larger than the number of objects given for string formatting\n"
		// 			"Please check your format again: " + convertedFmt + "\n" "Index: " + optionIndex + "Type: " + type;
		// 		throw kDebug::FormatError(errMsg);
		// 	}
		//
		// 	identifiers.push_back(FormatMarker(idx, openerPos, type));
		//
		// 	if (fmt.find_first_of(openerSymbol, openerPos + 1) != npos)
		// 		 CreateIdentifiers<CharType, Size, Ts...>(fmt, identifiers, currentIndex, args...);
		//
		// 	return identifiers;
		// }

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
