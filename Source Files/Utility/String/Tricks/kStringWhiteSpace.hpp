#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

#include "kStringLocale.hpp"

namespace klib::kString
{
	template<class CharT, class = std::enable_if_t<
		type_trait::Is_CharType_V<CharT>
		>>
		USE_RESULT constexpr bool IsWhiteSpace(CharT c, const std::string& localeName = "C")
	{
		const auto isWhiteSpace = IsIt(c, std::ctype_base::blank, localeName);
		return isWhiteSpace;
	}

	template<typename Stringish, class = std::enable_if_t<
		type_trait::Is_CString_V<Stringish>
		|| type_trait::Is_StringType_V<Stringish>
		>>
		USE_RESULT constexpr bool IsWhiteSpaceOrNull(const Stringish& str
			, const std::string& localeName = "C")
	{
		using namespace type_trait;
		if _CONSTEXPR17(Is_StringType_V<Stringish>)
		{
			if (str.data() == nullptr || str.empty())
				return false;

			for (const auto& c : str)
			{
				if (!IsWhiteSpace(c, localeName))
					return false;
			}
			return true;
		}
		else
		{
			if (str == nullptr)
				return false;

			auto index = 0u;
			while (str[index] != s_NullTerminator<ONLY_TYPE(Stringish)>)
			{
				if (!IsWhiteSpace(str[index], localeName))
					return false;
				++index;
			}

			return true;
		}
	}
}