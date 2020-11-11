#pragma once

#include "kStringTypes.hpp"
#include "../../Type Traits/StringTraits.hpp"

#include <algorithm>
#include <vector>
#include <regex>

namespace klib::kString
{
	enum class PreserveToken { YES, NO, };
	enum class PreserveEmpty { YES, NO, };

	template<class Char = char>
	USE_RESULT constexpr StringWriter<Char> Replace(const StringReader<Char>& str, const ONLY_TYPE(Char) oldChar, const ONLY_TYPE(Char) newChar) noexcept
	{
		using StrW = StringWriter<Char>;

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

	template<class Char = char>
	USE_RESULT constexpr std::vector<StringWriter<Char>> Split(const StringWriter<Char>& str, const Char* tokens,
		const PreserveToken preserveToken = PreserveToken::NO, const PreserveEmpty preserveEmpty = PreserveEmpty::NO)
	{
		using StrW = StringWriter<Char>;

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

	template<class Char = char>
	USE_RESULT constexpr std::vector<StringWriter<Char>> Split(const StringWriter<Char>& str, const StringReader<Char>& tokens,
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
		auto output = ToWriter( input );
		for (auto& c : output)
			c = ToUpper(c);
		return output;
	}

	template<class CharT, size_t Size, typename = std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		&& std::is_array_v<CharT[Size]>
		>>
	USE_RESULT constexpr auto ToUpper(const CharT (&input)[Size])
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

	template<typename StringType, typename = std::enable_if_t<type_trait::Is_StringType_V<StringType>>>
	USE_RESULT constexpr bool Contains(const StringType& str, const typename StringType::value_type* search
		, const size_t offset = 0)
	{
		return str.find(search, offset) != StringType::npos;
	}

	template<typename StringType, typename = std::enable_if_t<type_trait::Is_StringType_V<StringType>>>
	USE_RESULT constexpr bool Contains(const StringType& str, typename StringType::value_type search
		, const size_t offset = 0)
	{
		return str.find(search, offset) != StringType::npos;
	}

	template<typename StringA, typename StringB, typename = std::enable_if_t<
		type_trait::Is_StringType_V<StringA>
		&& type_trait::Is_StringType_V<StringB>
		>>
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
		USE_RESULT constexpr size_t Count(const StringType& str, const Stringish search)
	{
		size_t count = 0;

		for (auto currentPos = str.find_first_of(search);
			currentPos != StringType::npos;
			currentPos = str.find_first_of(search, currentPos + 1))
		{
			++count;
		}

		return count;
	}

	template<class Integer_t, typename StringType
		, typename = std::enable_if_t<
		type_trait::Is_StringType_V<StringType>
		>>
		constexpr Integer_t StrTo(StringType string)
	{
		static_assert(std::is_integral_v<Integer_t>, "Can only be used with integer types "
			"(char, int, long, unsigned short, etc..");

		using CharType = typename StringType::value_type;

		const auto CrashFunc = [](const std::string& errMsg) { throw std::runtime_error(errMsg); };

		std::regex r("\\s+");
		string = std::regex_replace(string, r, "");

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

		if (size > std::numeric_limits<Integer_t>::digits10)
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
}


