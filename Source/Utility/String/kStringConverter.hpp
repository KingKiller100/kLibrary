#pragma once

#include "../../HelperMacros.hpp"
#include "../../TypeTraits/StringTraits.hpp"
#include <string>

namespace klib
{
	namespace kString
	{
		template<typename DestChar, typename SourceChar, class = std::enable_if_t<
			type_trait::Is_Char_V<DestChar>
			&& type_trait::Is_Char_V<SourceChar>
			>>
			USE_RESULT constexpr DestChar Convert(SourceChar source) noexcept
		{
			return CAST(DestChar, source);
		}

		template<typename DestChar, typename SourceChar, class = std::enable_if_t<
			type_trait::Is_Char_V<DestChar>
			&& type_trait::Is_Char_V<SourceChar>
			>>
			USE_RESULT constexpr DestChar* Convert(const SourceChar* source) noexcept
		{
			if constexpr(std::is_same_v<SourceChar, DestChar>)
				return (DestChar*)source;
			else
			{
				constexpr auto destTerminator = type_trait::g_NullTerminator<DestChar>;
				constexpr auto sourceTerminator = type_trait::g_NullTerminator<SourceChar>;

				auto currentPos = source;
				size_t size = 0;

				while (*(currentPos++) != sourceTerminator)
					size++;

				auto result = new DestChar[size + 1]{ destTerminator };
				do {
					result[size] = Convert<DestChar>(source[size]);
				} while (size-- > 0);

				return result;
			}
		}

		template<class DestChar,
		typename DestTraits = std::char_traits<DestChar>,
		typename DestAllocT = std::allocator<DestChar>,
		class SourceChar = char,
		typename SourceTraits = std::char_traits<SourceChar>,
		typename SourceAllocT = std::allocator<SourceChar>,
		class = std::enable_if_t <
			type_trait::Is_Char_V<DestChar>
			&& type_trait::Is_Char_V<SourceChar>
			&& type_trait::Is_String_V<std::basic_string<DestChar>>
			&& type_trait::Is_String_V<std::basic_string<SourceChar>>
			>>
			USE_RESULT std::basic_string<DestChar, DestTraits, DestAllocT> Convert(const std::basic_string<SourceChar, SourceTraits, SourceAllocT>& source) noexcept
		{
			return Convert<DestChar>(source.data());
		}

		template<class DestChar,
		typename DestTraits = std::char_traits<DestChar>,
		typename DestAllocT = std::allocator<DestChar>,
		class SourceChar = char,
		typename SourceTraits = std::char_traits<SourceChar>,
		class = std::enable_if_t <
			type_trait::Is_Char_V<DestChar>
			&& type_trait::Is_Char_V<SourceChar>
			&& type_trait::Is_String_V<std::basic_string_view<DestChar>>
			&& type_trait::Is_String_V<std::basic_string_view<SourceChar>>
			>>
			USE_RESULT std::basic_string<DestChar, DestTraits, DestAllocT> Convert(const std::basic_string_view<SourceChar, SourceTraits>& source) noexcept
		{
			return Convert<DestChar>(source.data());
		}
	}
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif

}

#define kCHARACTERCONVERTSECRETIMPL(literal_prefix, string) (literal_prefix ## string)
#define U16_STR(string)  kCHARACTERCONVERTSECRETIMPL( u, string )
#define U32_STR(string)  kCHARACTERCONVERTSECRETIMPL( U, string )
#define WIDE_STR(string) kCHARACTERCONVERTSECRETIMPL( L, string )
#define U8_STR(string)   kCHARACTERCONVERTSECRETIMPL( u8, string )

