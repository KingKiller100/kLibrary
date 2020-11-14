#pragma once

#include "Stringify/kSprintf.hpp"
#include "Stringify/kFormatMarkers.hpp"
#include "Stringify/kStringIdentity.hpp"
#include "Stringify/kStringifyBool.hpp"
#include "Stringify/kStringifyFloatingPoint.hpp"
#include "Stringify/kStringifyInteger.hpp"
#include "Stringify/kStringifyPointer.hpp"

#include "../../TypeTraits/StringTraits.hpp"
#include "../../Utility/String/kStringConverter.hpp"

#include "Type/kStringUnsignedType.hpp"
#include "Type/kStringFloatingPointType.hpp"
#include "Type/kStringSimpleIntegerType.hpp"
#include "../Debug/Exceptions/StringExceptions.hpp"

#include <any>
#include <array>
#include <string>
#include <variant>


#include "Type/kStringBoolType.hpp"
#include "Type/kStringStringTypes.hpp"


namespace klib {
	namespace kString
	{
		template<typename CharType, std::size_t Size>
		secret::helper::FormatMarkerQueue CreateIdentifiers(const std::basic_string<CharType>& fmt, std::array<std::any, Size>& elems)
		{
			using namespace secret::helper;

			static constexpr auto openerSymbol = CharType('{');
			static constexpr auto closerSymbol = CharType('}');
			static constexpr auto specifierSymbol = CharType(':');
			static constexpr auto nullTerminator = type_trait::s_NullTerminator<CharType>;
			static constexpr auto npos = std::basic_string<CharType>::npos;

			FormatMarkerQueue identifiers;
			for (FormatMarker::PositionType openerPos = fmt.find_first_of(openerSymbol);
				openerPos != npos;
				openerPos = fmt.find_first_of(openerSymbol, openerPos + 1))
			{
				if (fmt[openerPos + 1] == openerSymbol ||
					fmt[openerPos + 1] == CharType(' ') ||
					fmt[openerPos + 1] == CharType('\t') ||
					fmt[openerPos + 1] == CharType('\n') ||
					fmt[openerPos + 1] == CharType('\r') ||
					fmt[openerPos + 1] == nullTerminator)
				{
					openerPos += 2;
					continue;
				}

				const auto closePos = fmt.find_first_of(closerSymbol, openerPos);
				const auto digits = closePos - openerPos - 1;
				const auto current = fmt.substr(openerPos + 1, digits);
				std::string bracketContents = kString::Convert<char>(current);

				const auto relativeColonPos = bracketContents.find_first_of(specifierSymbol);
				const auto optionIndex = bracketContents.substr(0, relativeColonPos);
				const auto idx = kString::StrTo<FormatMarker::IndexType>(optionIndex);
				if (elems.size() <= idx)
				{
					const auto convertedFmt = kString::Convert<char>(fmt);
					const auto errMsg = "Index given is larger than the number of objects given for string formatting\n"
						"Please check your format again: " + convertedFmt + "\n" "Index: " + optionIndex;
					throw std::out_of_range(errMsg);
				}

				const FormatMarker::NameType type = elems[idx].type().name();

				identifiers.push_back(FormatMarker(idx, openerPos, type));
			}
			identifiers.shrink_to_fit();

			return identifiers;
		}

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

			if (auto pfSymPos = format.find(printfSymbol); pfSymPos != npos)
			{
				return stringify::SprintfWrapper<CharType>(format, arg, argPack...);
			}

			std::array<std::any, std::variant_size_v<DataTypes> -1> elems = { stringify::IdentityPtr<CharType, T>(arg)
				, stringify::IdentityPtr<CharType, Ts>(argPack)... };

			std::basic_string<CharType> fmt(format);
			FormatMarkerQueue markers = CreateIdentifiers(ToWriter(format), elems);

			std::basic_string<CharType> finalString;
			size_t prevIndex = 0;
			for (const auto& marker : markers)
			{
				const std::any& val = elems[marker.objIndex];
				const auto& type = marker.type;
				const auto inputPos = fmt.find_first_of(closerSymbol, prevIndex) + 1;
				const auto endPos = inputPos - prevIndex;
				auto currentSection = fmt.substr(prevIndex, endPos);
				const auto replacePos = marker.position - prevIndex;
				const auto colonPos = currentSection.find(specifierSymbol, replacePos);

				std::basic_string<CharType> specifier;
				if (colonPos != npos)
				{
					const auto startPos = colonPos + 1;
					const auto count = (inputPos - 1) - startPos;
					specifier = currentSection.substr(colonPos + 1, count);
				}

				currentSection.erase(replacePos);

				if (Count(type, '*') > 1)
				{
					if (Contains(type, "char"))
					{
						const auto data = std::any_cast<const CharType* const*>(val);
						currentSection.append(*data);
					}
					else
					{
						const auto data = std::any_cast<const void* const*>(val);
						currentSection.append(stringify::StringifyPointer<CharType>(*data, specifier));
					}
				}
				else if (type::IsSTLString(type))
				{
					type::HandleStringType<CharType>(currentSection, type, val, specifier);
				}
				else if (type::IsUnsigned(type))
				{
					type::HandleUnsignedType<CharType>(currentSection, type, val, specifier);
				}
				else if (type::IsFloatingPoint(type))
				{
					type::HandleFloatingPointType<CharType>(currentSection, type, val, specifier);
				}
				else if (type::IsSimpleInteger(type))
				{
					type::HandleSimpleIntegerType<CharType>(currentSection, type, val, specifier);
				}
				else if (type::IsBool(type))
				{
					type::HandleBoolType<CharType>(currentSection, type, val, specifier);
				}
				else
				{
					const auto msg = stringify::SprintfWrapper(
						"Type \"%s\" is not recognised/supported by " __FUNCTION__
						, type);
					throw kDebug::FormatError(msg);
				}

				finalString.append(currentSection);
				prevIndex = inputPos;
				markers.pop_front();
			}

			if (fmt.size() - 1 >= prevIndex)
				finalString.append(fmt.substr(prevIndex));

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
