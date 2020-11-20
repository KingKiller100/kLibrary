#pragma once

#include "kStringTypes.hpp"
#include "../Enum/Enum.hpp"
#include "../Debug/Exceptions/StringExceptions.hpp"
#include "../../TypeTraits/StringTraits.hpp"

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

		ENUM_CLASS(PreserveToken, std::uint8_t, YES, NO);
		ENUM_CLASS(PreserveEmpty, std::uint8_t, YES, NO);
		ENUM_CLASS(CaseSensitive, std::uint8_t, YES, NO);

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
			const PreserveToken preserveToken = PreserveToken::NO, PreserveEmpty preserveEmpty = PreserveEmpty::NO)
		{
			using StrW = StringWriter<CharT>;

			const auto keepToken = preserveToken.Compare(PreserveToken::YES);
			const auto keepEmpty = preserveEmpty.Compare(PreserveEmpty::YES);

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

		template<typename StringType, typename Stringish
#if MSVC_PLATFORM_TOOLSET >= 142
			> requires ValidStringT<Stringish> || type_trait::Is_CharType_V<Stringish>
#else
			, typename = std::enable_if_t<type_trait::Is_StringType_V<Stringish> // STL string
			|| (type_trait::Is_CharType_V<ONLY_TYPE(Stringish)> // C string
				&& std::is_pointer_v<Stringish>)
			|| std::is_array_v<Stringish>
			|| type_trait::Is_CharType_V<Stringish>
			>>
#endif

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

		template<typename StringType
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_t<StringType>
#else
			, typename = std::enable_if_t<type_trait::Is_StringType_V<StringType>> >
#endif
			USE_RESULT constexpr size_t GetSize(const StringType& str)
		{
			return str.size();
		}

		template<typename CharT
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_Char_t<CharT>
#else
			, typename = std::enable_if_t<type_trait::Is_CharType_V<CharT>> >
#endif
			USE_RESULT constexpr size_t GetSize(const CharT* str)
		{
			size_t count = 0;
			while (*str != type_trait::s_NullTerminator<CharT>)
			{
				++str;
				++count;
			}
			return count;
		}

		template<class CharT = char>
		USE_RESULT constexpr std::vector<StringWriter<CharT>> Split(const StringWriter<CharT>& str, const StringReader<CharT>& tokens,
			const PreserveToken preserveToken = PreserveToken::NO, PreserveEmpty preserveEmpty = PreserveEmpty::NO)
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

			template<typename StringType
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_t<StringType>
#else
				, typename = std::enable_if_t<type_trait::Is_StringType_V<StringType>> >
#endif

				USE_RESULT constexpr bool Contains(const StringType& str, typename StringType::value_type search
					, const size_t offset = 0)
			{
				return str.find(search, offset) != StringType::npos;
			}

			template<typename StringA, typename StringB
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_t<StringA> && type_trait::Is_String_t<StringB>
#else
				, typename = std::enable_if_t<
				type_trait::Is_StringType_V<StringA>
				&& type_trait::Is_StringType_V<StringB>
				>>
#endif

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

				const auto hayStack = cs.Compare(CaseSensitive::YES, ToWriter(str), ToLower(str));

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

			template<class Integer_t, class Char_t, class = std::enable_if_t<
				type_trait::Is_CharType_V<Char_t>
				>>
				USE_RESULT constexpr Integer_t CStrTo(const Char_t* const str)
			{
				static_assert(std::is_integral_v<Integer_t>, __FUNCTION__ " can only be used with integer types "
					"(char, int, long, unsigned short, etc..");
			
				const auto CrashFunc = [](const std::string& errMsg) { throw kDebug::StringError(errMsg); };
				const auto MaxDigitsFunc = []()
				{

					auto maxNum = std::numeric_limits<Integer_t>::max();
					size_t count = 0;
					do {
						++count;
						maxNum /= 10;
					} while (maxNum);
					return count;
				};

				if (str == nullptr
					|| str[0] == type_trait::s_NullTerminator<Char_t>)
					return static_cast<Integer_t>(0);

				const auto isNeg = std::is_signed_v<Integer_t>
					&& str[0] == Char_t('-');

				Integer_t result = 0;
				auto currentPos = isNeg ? 1 : 0;
				size_t size = GetSize(str);
				auto magnitude = static_cast<size_t>(std::pow(10, size - 1));

				if (size > MaxDigitsFunc())
				{
					const std::string type = typeid(Integer_t).name();
					const auto msg = "String contains more digits than largest number of type: "
						+ type;
					CrashFunc(msg);
				}

				while (str[currentPos] != type_trait::s_NullTerminator<Char_t>)
				{
					if (Char_t('0') > str[currentPos]
						|| Char_t('9') < str[currentPos])
						CrashFunc("String must only contain digits");

					const auto digit = static_cast<size_t>(str[currentPos] - Char_t('0'));
					const auto asInt = digit * magnitude;

					result += static_cast<Integer_t>(asInt);
					magnitude /= 10;
					++currentPos;
				}

				if (isNeg)
					result *= -1;

				return result;
			}

			// Converts strings containing text for an integer value into the integer type with that value 
			template<class Integer_t, typename StringT
				, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<StringT, std::basic_string>
				>>
				USE_RESULT constexpr Integer_t StrTo(StringT string)
			{
				static_assert(std::is_integral_v<Integer_t>, __FUNCTION__ " can only be used with integer types "
					"(char, int, long, unsigned short, etc..");

				using CharType = typename StringT::value_type;

				const auto CrashFunc = [](const std::string& errMsg) { throw kDebug::StringError(errMsg); };

				Remove(string, ' ');

				if (string.empty())
					return static_cast<Integer_t>(0);

				if (Contains(string, CharType('.')))
					CrashFunc("string must contain only one integer number");

				const auto result = CStrTo<Integer_t>(string.data());

				return result;
			}

			// Converts strings containing text for an integer value into the integer type with that value.
			// But if zero results are unwanted, a default value can be returned instead
			template<class Integer_t, typename StringT
				, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<StringT, std::basic_string>
				>>
				USE_RESULT constexpr Integer_t StrTo(StringT string, Integer_t defaultValue)
			{
				const auto val = StrTo<Integer_t>(string);
				if (val == 0)
					return defaultValue;
				return val;
			}


			// Converts strings containing text for an integer value into the integer type with that value.
			// But bad string inputs are likely, ignore the exception and return a default value
			template<class Integer_t, typename StringT
				, typename = std::enable_if_t<
				type_trait::Is_Specialization_V<StringT, std::basic_string>
				>>
				USE_RESULT constexpr Integer_t TryStrTo(StringT string, Integer_t defaultValue)
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

