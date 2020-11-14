#pragma once

#include "kStringTypes.hpp"
#include "../Debug Helper/Exceptions/StringExceptions.hpp"
#include "../../Type Traits/StringTraits.hpp"

#include <algorithm>
#include <vector>


#ifdef max
#	undef max
#endif

namespace klib
{
	namespace kString
	{
#if MSVC_PLATFORM_TOOLSET >= 142
		template<typename Stringish>
		concept ValidStringT = type_trait::Is_StringType_V<Stringish> // STL string
			|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)> // C string
				&& std::is_pointer_v<Stringish>)
			|| std::is_array_v<Stringish>;
#endif
		
		enum class PreserveToken { YES, NO, };
		enum class PreserveEmpty { YES, NO, };
		enum class CaseSensitive { YES, NO, };

		template<class CharT = char>
		USE_RESULT constexpr StringWriter<CharT> Replace(const StringReader<CharT>& str, const ONLY_TYPE(CharT) oldChar, const ONLY_TYPE(CharT) newChar) noexcept
		{
			using StrW = StringWriter<CharT>;

			auto text = StrW(str);

#if MSVC_PLATFORM_TOOLSET >= 141
			std::replace(text.begin(), text.end(), oldChar, newChar);
#else
			auto oldCharPos = str.find_first_of(oldChar);

			if (oldCharPos == StrW::npos)
				return text;

			if (str[0] == oldChar)
				text[0] = oldChar;

			do {
				text[oldCharPos] = newChar;
				oldCharPos = text.find_first_of(oldChar);
			} while (oldCharPos != StrW::npos);
#endif
			return text;
		}

		template<class CharT = char>
		USE_RESULT constexpr std::vector<StringWriter<CharT>> Split(const StringWriter<CharT>& str, const CharT* tokens,
			const PreserveToken preserveToken = PreserveToken::NO, const PreserveEmpty preserveEmpty = PreserveEmpty::NO)
		{
			using StrW = StringWriter<CharT>;

			const auto keepToken = (preserveToken == PreserveToken::YES);
			const auto keepEmpty = (preserveEmpty == PreserveEmpty::YES);

			std::vector<StrW> lines;

			size_t prevPos = 0;
			size_t tokenPos = 0;
			while (tokenPos != StrW::npos)
			{
				tokenPos = str.find_first_of(tokens, prevPos);
				const auto line = str.substr(prevPos, (keepToken ? tokenPos + 1 : tokenPos) - prevPos);
				if (!line.empty() || keepEmpty)
					lines.emplace_back(line);
				prevPos = tokenPos + 1;
			}

			return lines;
		}
		
		template<typename StringType, typename Stringish>
		requires ValidStringT<Stringish> || type_trait::Is_CharType_V<Stringish>
			constexpr bool Remove(StringType & str, const Stringish & search)
		{
			bool removed = false;
			auto pos = str.find(search);

			while (pos != StringType::npos)
			{
				const auto endPos = str.find_first_not_of(search, pos);
				str.erase(pos, endPos - pos);
				pos = str.find(search);
				removed = true;
			}
			return removed;
		}

		template<class CharT = char>
		USE_RESULT constexpr std::vector<StringWriter<CharT>> Split(const StringWriter<CharT>& str, const StringReader<CharT>& tokens,
			const PreserveToken preserveToken = PreserveToken::NO, const PreserveEmpty preserveEmpty = PreserveEmpty::NO)
		{
			return Split(str, tokens.data(), preserveToken, preserveEmpty);
		}

		template<class CharT, class =
			std::enable_if_t<
			type_trait::Is_CharType_V<CharT>
			>>
			USE_RESULT constexpr CharT ToUpper(CharT c)
		{
			if (c >= CharT('a') && c <= CharT('z'))
				c -= 32;
			return c;
		}

		template<class Stringish
			, class = std::enable_if_t<
			type_trait::Is_StringType_V<Stringish>
			|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)>
				&& std::is_pointer_v<Stringish>)
			>>
			USE_RESULT constexpr auto ToUpper(const Stringish& input)
		{
			auto output = ToWriter(input);
			for (auto& c : output)
				c = ToUpper(c);
			return output;
		}

		template<class CharT, size_t Size, typename = std::enable_if_t<
			type_trait::Is_CharType_V<CharT>
			&& std::is_array_v<CharT[Size]>
			>>
			USE_RESULT constexpr auto ToUpper(const CharT(&input)[Size])
		{
			return ToUpper(StringReader<std::remove_all_extents_t<CharT>>(input));
		}

		template<class CharT, class =
			std::enable_if_t<
			type_trait::Is_CharType_V<CharT>
			>>
			USE_RESULT constexpr CharT ToLower(CharT c)
		{
			if (c >= CharT('A') && c <= CharT('Z'))
				c += 32;
			return c;
		}

		template<class Stringish
			, class = std::enable_if_t<
			type_trait::Is_StringType_V<Stringish>
			|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)>
				&& std::is_pointer_v<Stringish>)
			>>
			USE_RESULT constexpr auto ToLower(const Stringish& input)
		{
			auto output = ToWriter(input);
			for (auto& c : output)
				c = ToLower(c);
			return output;
		}

		template<class CharT, size_t Size, typename = std::enable_if_t<
			type_trait::Is_CharType_V<CharT>
			&& std::is_array_v<CharT[Size]>
			>> 
			USE_RESULT constexpr auto ToLower(const CharT(&input)[Size])
		{
			return ToLower(StringReader<std::remove_all_extents_t<CharT>>(input));
		}

#if MSVC_PLATFORM_TOOLSET < 142
		template<typename StringType, typename = std::enable_if_t<type_trait::Is_StringType_V<StringType>>>
#else
		template<typename StringType> requires type_trait::Is_String_t<StringType>
#endif
		USE_RESULT constexpr bool Contains(const StringType& str, const typename StringType::value_type* search
			, const size_t offset = 0)
		{
			return str.find(search, offset) != StringType::npos;
		}

		template<typename StringType> requires type_trait::Is_String_t<StringType>
		USE_RESULT constexpr bool Contains(const StringType& str, typename StringType::value_type search
			, const size_t offset = 0)
		{
			return str.find(search, offset) != StringType::npos;
		}

		template<typename StringA, typename StringB>
		requires type_trait::Is_String_t<StringA> && type_trait::Is_String_t<StringB>
			USE_RESULT constexpr bool Contains(const StringA& str, const StringB& search
				, const size_t offset = 0)
		{
			return str.find(search.data(), offset) != StringA::npos;
		}

		template<typename StringType, typename Stringish
			, typename = std::enable_if_t<
			type_trait::Is_StringType_V<StringType>
			&& (type_trait::Is_StringType_V<StringType>
				|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)>
					&& std::is_same_v<typename StringType::value_type, Stringish>
					&& std::is_pointer_v<Stringish>)
				)
			>>
			USE_RESULT constexpr size_t Count(const StringType& str, const Stringish search
				, const size_t offset = 0, CaseSensitive cs = CaseSensitive::NO)
		{
			size_t count = 0;

			const auto hayStack = cs == CaseSensitive::YES
				? ToWriter(str)
				: ToLower(str);

			const auto needle = cs == CaseSensitive::YES
				? search
				: ToLower(search);
			
			for (auto currentPos = hayStack.find(needle, offset);
				currentPos != StringType::npos;
				currentPos = hayStack.find(needle, currentPos + 1))
			{
				++count;
			}

			return count;
		}

		template<class Integer_t, typename StringT
			, typename = std::enable_if_t<
			std::_Is_specialization_v<StringT, std::basic_string>
			>>
			USE_RESULT constexpr Integer_t StrTo(StringT string)
		{
			static_assert(std::is_integral_v<Integer_t>, __FUNCTION__ " can only be used with integer types "
				"(char, int, long, unsigned short, etc..");

			using CharType = typename StringT::value_type;

			const auto CrashFunc = [](const std::string& errMsg) { throw kDebug::StringError(errMsg); };
			const auto MaxDigitsFunc = []()
			{
				auto maxNum = std::numeric_limits<Integer_t>::max();
				size_t count = 0;
				do {
					++count;
					maxNum /= 10;
				}
				while (maxNum);
				return count;
			};

			
			Remove(string, ' ');

			if (string.empty())
				CrashFunc("Cannot convert empty string to integer number");
			
			if (Contains(string, CharType('.')))
				CrashFunc("string must contain only one integer number");

			const auto isNeg = std::is_signed_v<Integer_t>
				&& string[0] == CharType('-');

			if (isNeg)
				string.erase(string.begin(), string.begin() + 1);

			Integer_t result = 0;
			size_t size = string.size();
			auto magnitude = static_cast<size_t>(std::pow(10, size - 1));

			if (size > MaxDigitsFunc())
			{
				const std::string type = typeid(result).name();
				const auto msg = "String contains more digits than largest number of type: "
					+ type;
				CrashFunc(msg);
			}

			for (const auto& digitChr : string)
			{
				if (CharType('0') > digitChr
					|| CharType('9') < digitChr)
					CrashFunc("String must only contain digits");

				const auto digit = static_cast<size_t>(digitChr - CharType('0'));
				const auto asInt = digit * magnitude;

				result += static_cast<Integer_t>(asInt);
				magnitude /= 10;
			}

			if (isNeg)
				result *= -1;

			return result;
		}

		template<class Integer_t, typename StringT
			, typename = std::enable_if_t<
			std::_Is_specialization_v<StringT, std::basic_string>
			>>
			USE_RESULT constexpr Integer_t StrTo(StringT string, Integer_t defaultValue)
		{
			try
			{
				return StrTo<Integer_t>(string);
			}
			catch (...)
			{
				return defaultValue;
			}
		}
		
	}
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif
}

