#pragma once

#include "kStringTypes.hpp"
#include "../Enum/kEnum.hpp"
#include "../../TypeTraits/StringTraits.hpp"

#include "Tricks/kStringOperators.hpp"
#include "Tricks/kStringFind.hpp"
#include "Tricks/kStringSize.hpp"
#include "Tricks/kStringCases.hpp"
#include "Tricks/kStringWhiteSpace.hpp"
#include "Tricks/kStringRemove.hpp"
#include "Tricks/kStringTo.hpp"
#include "Tricks/kStringData.hpp"

#include <algorithm>
#include <vector>


namespace klib
{
	namespace kString
	{
		ENUM_CLASS(PreserveToken, std::uint8_t, YES, NO);
		ENUM_CLASS(PreserveEmpty, std::uint8_t, YES, NO);

		template<class CharT = char, class = std::enable_if_t<
			type_trait::Is_Char_V<CharT>
			>>
		USE_RESULT constexpr StringWriter<CharT> Replace(const CharT* str, CharT oldChar, CharT newChar) noexcept
		{
			using StrW = StringWriter<CharT>;

			auto text = StrW(str);

#if MSVC_PLATFORM_TOOLSET >= 141
			std::replace(text.begin(), text.end(), oldChar, newChar);
#else
			auto oldCharPos = Find_First_Of(str, oldChar);

			if (oldCharPos == StrW::npos)
				return text;

			if (str[0] == oldChar)
				text[0] = oldChar;

			do {
				text[oldCharPos] = newChar;
				oldCharPos = Find_First_Of(text.data(), oldChar);
			} while (oldCharPos != StrW::npos);
#endif
			return text;
		}

		template<class StringT, typename = std::enable_if_t<
			type_trait::Is_String_V<StringT>
		>>
		USE_RESULT constexpr StringWriter<typename StringT::value_type> Replace(const StringT& str, typename StringT::value_type oldChar, typename StringT::value_type newChar) noexcept
		{
			return Replace<typename StringT::value_type>(str.data(), oldChar, newChar);
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

		template<class CharT = char>
		USE_RESULT constexpr std::vector<StringWriter<CharT>> Split(const StringWriter<CharT>& str, const StringReader<CharT>& tokens,
			const PreserveToken preserveToken = PreserveToken::NO, PreserveEmpty preserveEmpty = PreserveEmpty::NO)
		{
			return Split(str, tokens.data(), preserveToken, preserveEmpty);
		}

	}
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif
}

