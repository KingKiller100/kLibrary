#pragma once

#include "../../../TypeTraits/StringTraits.hpp"
#include "../../String/kStringConverter.hpp"
#include <string>

namespace klib {
	namespace kDebug
	{
		// Non-Mutable Source Code Object
		template<class Char>
		struct BasicSourceInfo
		{
		public:
			using Char_t = Char;

			template<typename StringishA_t, typename StringishB_t, typename StringishC_t, class = std::enable_if_t<
				type_trait::Is_Stringish_V<StringishA_t>
				&& type_trait::Is_Stringish_V<StringishB_t>
				&& type_trait::Is_Stringish_V<StringishC_t>
				>>
				constexpr BasicSourceInfo(const StringishA_t& filename, const size_t fileLine
					, const StringishB_t& function, const StringishC_t& fileStamp = __TIMESTAMP__)
				: file(filename)
				, line(fileLine)
				, func(function)
				, stamp(fileStamp)
			{}

			template<typename StringishA_t, typename StringishB_t, class = std::enable_if_t<
				type_trait::Is_Stringish_V<StringishA_t>
				&& type_trait::Is_Stringish_V<StringishB_t>
				>>
				constexpr BasicSourceInfo(const StringishA_t& filename, const size_t fileLine
					, const StringishB_t& function)
				: file(filename)
				, line(fileLine)
				, func(function)
				, stamp(type_trait::EmptyString<Char>())
			{}

		public:
			const std::basic_string_view<Char_t> file;
			const size_t line;
			const std::basic_string_view<Char_t> func;
			const std::basic_string_view<Char_t> stamp;
		};

		using SourceInfo = BasicSourceInfo<char>;
		using wSourceInfo = BasicSourceInfo<wchar_t>;
		using u16SourceInfo = BasicSourceInfo<char16_t>;
		using u32SourceInfo = BasicSourceInfo<char32_t>;
#ifdef __cpp_char8_t
		using u8SourceInfo = BasicSourceInfo<char8_t>;
#endif
	}

#ifdef KLIB_SHORT_NAMESPACE
		using namespace kDebug;
#endif
}

#define SOURCE_INFO()              ::klib::kDebug::SourceInfo(__FILE__, kFILELINE, __FUNCTION__, __TIMESTAMP__)
#define WSOURCE_INFO()             ::klib::kDebug::wSourceInfo(WIDE_STR(__FILE__), kFILELINE, WIDE_STR(__FUNCTION__), WIDE_STR(__TIMESTAMP__))
#define U16SOURCE_INFO()           ::klib::kDebug::u16SourceInfo(U16_STR(__FILE__), kFILELINE, U16_STR(__FUNCTION__), U16_STR(__TIMESTAMP__))
#define U32SOURCE_INFO()           ::klib::kDebug::u32SourceInfo(U32_STR(__FILE__), kFILELINE, U32_STR(__FUNCTION__), U32_STR(__TIMESTAMP__))

#ifdef __cpp_char8_t
#	define U8SOURCE_INFO()         ::klib::kDebug::u8SourceInfo(U8_STR(__FILE__), kFILELINE, U8_STR(__FUNCTION__), U8_STR(__TIMESTAMP__))
#endif

#define SOURCE_INFO_NO_FUNC()      ::klib::kDebug::SourceInfo(__FILE__, kFILELINE, "", __TIMESTAMP__)
#define WSOURCE_INFO_NO_FUNC()     ::klib::kDebug::wSourceInfo(__FILEW__, kFILELINE, L"", WIDE_STR(__TIMESTAMP__))
#define U16SOURCE_INFO_NO_FUNC()   ::klib::kDebug::u16SourceInfo(U16_STR(__FILE__), kFILELINE, u"", U16_STR(__TIMESTAMP__))
#define U32SOURCE_INFO_NO_FUNC()   ::klib::kDebug::u32SourceInfo(U32_STR(__FILE__), kFILELINE, U"", U32_STR(__TIMESTAMP__))

#ifdef __cpp_char8_t
#	define U8SOURCE_INFO_NO_FUNC() ::klib::kDebug::u8SourceInfo(U8_STR(__FILE__), kFILELINE, u8"", U8_STR(__TIMESTAMP__))
#endif

#ifdef MSVC_PLATFORM_TOOLSET
#	define SOURCE_INFO_VS()        ::klib::kDebug::SourceInfo(__FILE__, kFILELINE, __FUNCSIG__, __TIMESTAMP__)
#	define WSOURCE_INFO_VS()       ::klib::kDebug::wSourceInfo(__FILEW__, kFILELINE, WIDE_STR(__FUNCSIG__), WIDE_STR(__TIMESTAMP__))
#	define U16SOURCE_INFO_VS()     ::klib::kDebug::u16SourceInfo(U16_STR(__FILE__), kFILELINE,  U16_STR(__FUNCSIG__), U16_STR(__TIMESTAMP__))
#	define U32SOURCE_INFO_VS()     ::klib::kDebug::u32SourceInfo(U32_STR(__FILE__), kFILELINE,  U32_STR(__FUNCSIG__), U32_STR(__TIMESTAMP__))
#	ifdef __cpp_char8_t
#		define U8SOURCE_INFO_VS() ::klib::kDebug::u8SourceInfo(U8_STR(__FILE__), kFILELINE,  U8_STR(__FUNCSIG__), U8_STR(__TIMESTAMP__))
#	endif
#endif
