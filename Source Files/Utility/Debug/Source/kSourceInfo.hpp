#pragma once

#include "../../../TypeTraits/StringTraits.hpp"
#include "../../String/kStringConverter.hpp"
#include <string>

namespace klib {
	namespace kDebug
	{
		template<class Char, class = std::enable_if_t<type_trait::Is_CharType_V<Char>>>
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
					, const StringishB_t& function, const StringishC_t& modificationTimeStamp)
				: file(filename)
				, line(fileLine)
				, func(function)
				, timeStamp(modificationTimeStamp)
			{}

			USE_RESULT std::basic_string<Char_t> ToString() const;

		public:
			const std::basic_string_view<Char_t> file;
			const size_t line;
			const std::basic_string_view<Char_t> func;
			const std::basic_string_view<Char_t> timeStamp;
		};

		using SourceInfo = BasicSourceInfo<char>;
		using wSourceInfo = BasicSourceInfo<wchar_t>;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

#define kSOURCEINFOIMPL_CONCATENATE(x,y) kSOURCEINFOIMPL_CONCATENATE2(x,y) 
#define kSOURCEINFOIMPL_CONCATENATE2(x,y) x##y 
#define kSOURCEINFOIMPL_FILELINE size_t(kSOURCEINFOIMPL_CONCATENATE(__LINE__, U))

#define SOURCE_INFO()          ::klib::kDebug::SourceInfo(__FILE__, kSOURCEINFOIMPL_FILELINE, __FUNCTION__, __TIMESTAMP__)

#define SOURCE_INFO_NO_FUNC()  ::klib::kDebug::SourceInfo(__FILE__, kSOURCEINFOIMPL_FILELINE, "", __TIMESTAMP__)

#define WSOURCE_INFO_NO_FUNC() ::klib::kDebug::wSourceInfo(__FILEW__, kSOURCEINFOIMPL_FILELINE, L"", WIDE_STR(__TIMESTAMP__))
#define WSOURCE_INFO()         ::klib::kDebug::wSourceInfo(__FILEW__, kSOURCEINFOIMPL_FILELINE, __FUNCTIONW__, WIDE_STR(__TIMESTAMP__))

#ifdef MSVC_PLATFORM_TOOLSET
#	define SOURCE_INFO_VS()    ::klib::kDebug::SourceInfo(__FILE__, kSOURCEINFOIMPL_FILELINE, __FUNCSIG__, __TIMESTAMP__)
#	define WSOURCE_INFO_VS()    ::klib::kDebug::SourceInfo(__FILE__, kSOURCEINFOIMPL_FILELINE, _CRT_WIDE(__FUNCSIG__), WIDE_STR(__TIMESTAMP__))
#endif
