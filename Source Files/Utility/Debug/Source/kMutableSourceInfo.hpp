#pragma once

#include "../../FileSystem/kFileSystemTypes.hpp"

#include "../../../TypeTraits/StringTraits.hpp"
#include "../../String/kStringConverter.hpp"
#include <string>

namespace klib {
	namespace kDebug
	{
		// Mutable Source Code Object
		template<class Char>
		struct BasicMutableSourceInfo
		{
		public:
			using Char_t = Char;

			template<typename StringishA_t, typename StringishB_t, class = std::enable_if_t<
				type_trait::Is_Stringish_V<StringishA_t>
				&& type_trait::Is_Stringish_V<StringishB_t>
				>>
				constexpr BasicMutableSourceInfo(const std::filesystem::path& filename, const size_t fileLine
					, const StringishA_t& function, const StringishB_t& modificationTimeStamp)
				: file(filename)
				, line(fileLine)
				, func(function)
				, timeStamp(modificationTimeStamp)
			{}

		public:
			kFileSystem::Path file;
			size_t line;
			std::basic_string<Char_t> func;
			std::basic_string<Char_t> timeStamp;
		};

		using MutSourceInfo = BasicMutableSourceInfo<char>;
		using wMutSourceInfo = BasicMutableSourceInfo<wchar_t>;
		using u16MutSourceInfo = BasicMutableSourceInfo<char16_t>;
		using u32MutSourceInfo = BasicMutableSourceInfo<char32_t>;
#ifdef __cpp_char8_t
		using u8MutSourceInfo = BasicMutableSourceInfo<char8_t>;
#endif
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

#define MUT_SRC_INFO()              ::klib::kDebug::MutSourceInfo(__FILE__, kFILELINE, __FUNCTION__, __TIMESTAMP__)
#define WMUT_SRC_INFO()             ::klib::kDebug::wMutSourceInfo(WIDE_STR(__FILE__), kFILELINE, WIDE_STR(__FUNCTION__), WIDE_STR(__TIMESTAMP__))
#define U16MUT_SRC_INFO()           ::klib::kDebug::u16MutSourceInfo(U16_STR(__FILE__), kFILELINE, U16_STR(__FUNCTION__), U16_STR(__TIMESTAMP__))
#define U32MUT_SRC_INFO()           ::klib::kDebug::u32MutSourceInfo(U32_STR(__FILE__), kFILELINE, U32_STR(__FUNCTION__), U32_STR(__TIMESTAMP__))

#ifdef __cpp_char8_t
#	define U8MUT_SRC_INFO()         ::klib::kDebug::u8MutSourceInfo(U8_STR(__FILE__), kFILELINE, U8_STR(__FUNCTION__), U8_STR(__TIMESTAMP__))
#endif

#define MUT_SRC_INFO_NO_FUNC()      ::klib::kDebug::MutSourceInfo(__FILE__, kFILELINE, "", __TIMESTAMP__)
#define WMUT_SRC_INFO_NO_FUNC()     ::klib::kDebug::wMutSourceInfo(__FILEW__, kFILELINE, L"", WIDE_STR(__TIMESTAMP__))
#define U16MUT_SRC_INFO_NO_FUNC()   ::klib::kDebug::u16MutSourceInfo(U16_STR(__FILE__), kFILELINE, u"", U16_STR(__TIMESTAMP__))
#define U32MUT_SRC_INFO_NO_FUNC()   ::klib::kDebug::u32MutSourceInfo(U32_STR(__FILE__), kFILELINE, U"", U32_STR(__TIMESTAMP__))

#ifdef __cpp_char8_t
#	define U8MUT_SRC_INFO_NO_FUNC() ::klib::kDebug::u8MutSourceInfo(U8_STR(__FILE__), kFILELINE, u8"", U8_STR(__TIMESTAMP__))
#endif

#ifdef MSVC_PLATFORM_TOOLSET
#	define MUT_SRC_INFO_VS()        ::klib::kDebug::MutSourceInfo(__FILE__, kFILELINE, __FUNCSIG__, __TIMESTAMP__)
#	define WMUT_SRC_INFO_VS()       ::klib::kDebug::wMutSourceInfo(__FILEW__, kFILELINE, WIDE_STR(__FUNCSIG__), WIDE_STR(__TIMESTAMP__))
#	define U16MUT_SRC_INFO_VS()     ::klib::kDebug::u16MutSourceInfo(U16_STR(__FILE__), kFILELINE,  U16_STR(__FUNCSIG__), U16_STR(__TIMESTAMP__))
#	define U32MUT_SOURCE_INFO_VS()     ::klib::kDebug::u32MutSourceInfo(U32_STR(__FILE__), kFILELINE,  U32_STR(__FUNCSIG__), U32_STR(__TIMESTAMP__))
#	ifdef __cpp_char8_t
#		define U8MUT_SRC_INFO_NO_FUNC() ::klib::kDebug::u8MutSourceInfo(U8_STR(__FILE__), kFILELINE,  U8_STR(__FUNCSIG__), U8_STR(__TIMESTAMP__))
#	endif
#endif
