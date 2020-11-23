#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"


namespace klib::kString
{
	template<typename CStringA, typename CStringB, class = std::enable_if_t<
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CString_V<CStringB>
		&& std::is_same_v<ONLY_TYPE(CStringA), ONLY_TYPE(CStringB)>
		>>
		USE_RESULT constexpr size_t Find(const CStringA& str, const CStringB& search, size_t offset = 0)
	{
		using namespace type_trait;
		using Char_t = typename type_trait::Is_CString<CStringA>::Char_t;
		using PossibleString_t = std::basic_string<Char_t>;

		constexpr Char_t nt = s_NullTerminator<Char_t>;
		constexpr size_t npos = s_NoPos<PossibleString_t>;

		if (str == nullptr || search == nullptr)
			return npos;

		const auto searchSize = GetSize(search);

		auto strSize = GetSize(str);
		if (offset >= strSize) return npos;
		strSize = GetSize(str + offset);

		if (searchSize > strSize) // if search string longer than size of pointer
			return npos;

		size_t strIndex(offset);
		size_t searchIndex(0);

		while (str[strIndex] != nt)
		{
			if (str[strIndex] == search[searchIndex]
				&& search[searchIndex] != nt)
			{
				++strIndex;
				++searchIndex;
				continue;
			}

			if (search[searchIndex] == nt)
				return strIndex - searchIndex;

			++strIndex;
			searchIndex = 0;
		}

		if (search[searchIndex] == nt)
			return strIndex - searchIndex;

		return npos;
	}

	template<typename CStringA, typename Char_t, typename CmpFunc, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CharType_V<Char_t>
		&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
		>>
		USE_RESULT constexpr size_t FindCharImpl(const CStringA& str, Char_t search, size_t offset, CmpFunc&& func)
	{
		using namespace type_trait;
		using PossibleString_t = std::basic_string<Char_t>;

		constexpr Char_t nt = s_NullTerminator<Char_t>;
		constexpr size_t npos = s_NoPos<PossibleString_t>;

		if (str == nullptr)
			return npos;

		auto strSize = GetSize(str);
		if (offset >= strSize) return npos;
		strSize = GetSize(str + offset);

		size_t strIndex(offset);
		while (str[strIndex] != nt)
		{
			if (func(str[strIndex], search))
				return strIndex;

			++strIndex;
		}

		return npos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CharType_V<Char_t>
		&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
		>>
		USE_RESULT constexpr size_t Find_First_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		const size_t pos = FindCharImpl(str, search, offset, std::equal_to<Char_t>());
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CharType_V<Char_t>
		&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
		>>
		USE_RESULT constexpr size_t Find_First_Not_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		const size_t pos = FindCharImpl(str, search, offset, std::not_equal_to<Char_t>{});
		return pos;
	}
}
