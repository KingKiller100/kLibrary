#pragma once
#include "kStringSize.hpp"
#include "kStringCases.hpp"

#include "../../../Template/kComparisonOperators.hpp"
#include "../../../Template/kArthimeticOperators.hpp"

#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"

namespace klib::kString
{
	namespace secret::impl
	{
		template<typename CStringA, typename Char_t, typename CmpFunc_t, typename DirectionFunc_t, class = std::enable_if_t <
			type_trait::Is_CString_V<CStringA>
			&& type_trait::Is_Char_V<Char_t>
			&& std::is_same_v<klib::type_trait::Simplify_t<CStringA>, Char_t>
			>>
			USE_RESULT constexpr size_t FindCharImpl(const CStringA& hayStack, Char_t needle, size_t offset, CmpFunc_t&& cmpFunc, DirectionFunc_t&& directionFunc)
		{
			using namespace type_trait;
			using PossibleString_t = std::basic_string<Char_t>;

			constexpr Char_t nt = g_NullTerminator<Char_t>;
			constexpr size_t npos = g_NoPos<PossibleString_t>;

			for (std::make_signed_t<size_t> index(offset); 
				hayStack[index] != nt && index >= 0; 
				directionFunc(index))
			{
				if (cmpFunc(hayStack[index], needle))
					return index;
			}

			return npos;
		}
	}

	template<typename CStringA, typename CStringB, class = std::enable_if_t<
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_CString_V<CStringB>
		&& std::is_same_v<type_trait::Simplify_t<CStringA>, type_trait::Simplify_t<CStringB>>
		>>
		USE_RESULT constexpr size_t Find(const CStringA hayStack, const CStringB needle, size_t offset = 0)
	{
		using namespace type_trait;
		using Char_t = typename type_trait::Is_CString<CStringA>::Char_t;
		using PossibleString_t = std::basic_string<Char_t>;

		constexpr Char_t nt = g_NullTerminator<Char_t>;
		constexpr size_t npos = g_NoPos<PossibleString_t>;

		std::make_signed_t<size_t> hIndex = offset, nIndex = 0;
		for (; (hayStack[hIndex] != nt && needle[nIndex] != nt)
			&& hIndex >= 0;
			++hIndex, ++nIndex)
		{
			if (hayStack[hIndex] != needle[nIndex])
				nIndex = -1;
		}

		if (needle[nIndex] == nt)
			return (hIndex - nIndex);

		return npos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_Char_V<Char_t>
		&& std::is_same_v<type_trait::Simplify_t<CStringA>, Char_t>
		>>
		USE_RESULT constexpr size_t Find_First_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		using namespace kTemplate;
		const size_t pos = secret::impl::FindCharImpl(str, search, offset, ComparisonOperators::Equality, ArithmeticOperators::Increment);
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_Char_V<Char_t>
		&& std::is_same_v<type_trait::Simplify_t<CStringA>, Char_t>
		>>
		USE_RESULT constexpr size_t Find(const CStringA& str, Char_t search, size_t offset = 0)
	{
		const size_t pos = Find_First_Of(str, search, offset);
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_Char_V<Char_t>
		&& std::is_same_v<type_trait::Simplify_t<CStringA>, Char_t>
		>>
		USE_RESULT constexpr size_t Find_First_Not_Of(const CStringA& str, Char_t search, size_t offset = 0)
	{
		using namespace kTemplate;
		const size_t pos = secret::impl::FindCharImpl(str, search, offset, ComparisonOperators::Inequality, ArithmeticOperators::Increment);
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_Char_V<Char_t>
		&& std::is_same_v<type_trait::Simplify_t<CStringA>, Char_t>
		>>
		USE_RESULT constexpr size_t Find_Last_Of(const CStringA& str, Char_t search, size_t offset = type_trait::g_NoPos<std::basic_string<Char_t>>)
	{
		using namespace kTemplate;
		offset = std::min(offset, GetSize(str) - 1);
		const size_t pos = secret::impl::FindCharImpl(str, search, offset, ComparisonOperators::Equality, ArithmeticOperators::Decrement);
		return pos;
	}

	template<typename CStringA, typename Char_t, class = std::enable_if_t <
		type_trait::Is_CString_V<CStringA>
		&& type_trait::Is_Char_V<Char_t>
		&& std::is_same_v<type_trait::Simplify_t<CStringA>, Char_t>
		>>
		USE_RESULT constexpr size_t Find_Last_Not_Of(const CStringA& str, Char_t search, size_t offset = type_trait::g_NoPos<std::basic_string<Char_t>>)
	{
		using namespace kTemplate;
		offset = std::min(offset, GetSize(str) - 1);
		const size_t pos = secret::impl::FindCharImpl(str, search, offset, ComparisonOperators::Inequality, ArithmeticOperators::Decrement);
		return pos;
	}

#if MSVC_PLATFORM_TOOLSET < 142
	template<typename StringType, typename = std::enable_if_t<type_trait::Is_String_V<StringType>>>
#else
	template<typename StringType> requires type_trait::Is_String_c<StringType>
#endif
		USE_RESULT constexpr bool Contains(const StringType& str, const typename StringType::value_type* search
			, const size_t offset = 0)
		{
			return Find(str.data(), search, offset) != type_trait::g_NoPos<StringType>;
		}

		template<typename StringType
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_c<StringType>
#else
			, typename = std::enable_if_t<type_trait::Is_String_V<StringType>> >
#endif

			USE_RESULT constexpr bool Contains(const StringType & str, typename StringType::value_type search
				, const size_t offset = 0)
		{
			return Find_First_Of(str.data(), search, offset) != type_trait::g_NoPos<StringType>;
		}

		template<typename StringA, typename StringB
#if MSVC_PLATFORM_TOOLSET >= 142
				> requires type_trait::Is_String_c<StringA>&& type_trait::Is_String_c<StringB>
#else
			, typename = std::enable_if_t<
			type_trait::Is_String_V<StringA>
			&& type_trait::Is_String_V<StringB>
			>>
#endif
			USE_RESULT constexpr bool Contains(const StringA & str, const StringB & search
				, const size_t offset = 0)
		{
			return Find(str.data(), search.data(), offset) != type_trait::g_NoPos<StringA>;
		}

		template<typename StringType, typename Stringish
			, typename = std::enable_if_t<
			type_trait::Is_String_V<StringType>
			&& (type_trait::Is_String_V<StringType>
				|| (type_trait::Is_Char_V<type_trait::Simplify_t<Stringish>>
					&& std::is_same_v<typename StringType::value_type, Stringish>
					&& std::is_pointer_v<Stringish>)
				)
			>>
			USE_RESULT constexpr size_t Count(const StringType& str, const Stringish search
				, const size_t offset = 0, CaseSensitive cs = CaseSensitive::NO)
		{
			size_t count = 0;

			const auto hayStack = cs.Compare(CaseSensitive::YES
				, ToWriter(str)
				, ToLower(str));

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
