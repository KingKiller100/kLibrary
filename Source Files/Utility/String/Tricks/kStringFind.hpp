#pragma once
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "kStringSize.hpp"

namespace klib::kString
{
	ENUM_CLASS(CaseSensitive, std::uint8_t, YES, NO);

	namespace impl
	{
		template<typename CStringA, typename Char_t, typename CmpFunc_t, typename DirectionFunc_t, class = std::enable_if_t <
			type_trait::Is_CString_V<CStringA>
			&& type_trait::Is_CharType_V<Char_t>
			&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
			>>
			USE_RESULT constexpr size_t FindCharImpl(const CStringA& str, Char_t search, size_t offset, CmpFunc_t&& cmpFunc, DirectionFunc_t&& directionFunc)
		{
			using namespace type_trait;
			using PossibleString_t = std::basic_string<Char_t>;

			constexpr Char_t nt = s_NullTerminator<Char_t>;
			constexpr size_t npos = s_NoPos<PossibleString_t>;

			if (str == nullptr)
				return npos;

			auto strSize = GetSize(str);
			if (offset >= strSize) return npos;

			size_t strIndex(offset);
			while (str[strIndex] != nt)
			{
				if (cmpFunc(str[strIndex], search))
					return strIndex;

				strIndex = directionFunc(strIndex, 1);
			}

			return npos;
		}
	}
	
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

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CharType_V<Char_t>
		&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
		>>
		USE_RESULT constexpr size_t Find_First_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		const size_t pos = impl::FindCharImpl(str, search, offset, std::equal_to<Char_t>(), std::plus<void>{});
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CharType_V<Char_t>
		&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
		>>
		USE_RESULT constexpr size_t Find_First_Not_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		const size_t pos = impl::FindCharImpl(str, search, offset, std::not_equal_to<Char_t>{}, std::plus<void>{});
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CharType_V<Char_t>
		&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
		>>
		USE_RESULT constexpr size_t Find_Last_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		const auto size = GetSize(str) - 1;
		offset = size - offset;
		const size_t pos = impl::FindCharImpl(str, search, offset, std::equal_to<Char_t>{}, std::minus<void>{});
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CharType_V<Char_t>
		&& std::is_same_v<ONLY_TYPE(CStringA), Char_t>
		>>
		USE_RESULT constexpr size_t Find_Last_Not_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		const auto size = GetSize(str) - 1;
		offset = size - offset;
		const size_t pos = impl::FindCharImpl(str, search, offset, std::not_equal_to<Char_t>{}, std::minus<void>{});
		return pos;
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

			USE_RESULT constexpr bool Contains(const StringType & str, typename StringType::value_type search
				, const size_t offset = 0)
		{
			return str.find(search, offset) != type_trait::s_NoPos<StringType>;
		}

		template<typename StringA, typename StringB
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_t<StringA>&& type_trait::Is_String_t<StringB>
#else
			, typename = std::enable_if_t<
			type_trait::Is_StringType_V<StringA>
			&& type_trait::Is_StringType_V<StringB>
			>>
#endif

			USE_RESULT constexpr bool Contains(const StringA & str, const StringB & search
				, const size_t offset = 0)
		{
			return str.find(search.data(), offset) != type_trait::s_NoPos<StringA>;
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
}
