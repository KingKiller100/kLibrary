#pragma once

#include "../../../TypeTraits/CharacterTraits.hpp"
#include "../../String/kStringConverter.hpp"
#include <cstdint>

namespace klib::kDebug
{
	template<class Char, class = std::enable_if_t<type_trait::Is_CharType_V<Char>>>
	struct BasicSourceInfo
	{
		using Char_t = Char;
		
		const Char_t* file;
		const std::int32_t line;
		const Char_t* func;

		constexpr BasicSourceInfo() = default;
		
		constexpr BasicSourceInfo(const Char_t* filename, const std::int32_t fileLine, const Char_t* function)
			: file(filename)
			, line(fileLine)
			, func(function)
		{}

		template<class Char2>
		constexpr BasicSourceInfo(const Char2* filename, const std::int32_t fileLine, const Char2* function)
			: file(kString::Convert<Char_t>(filename))
			, line(fileLine)
			, func(kString::Convert<Char_t>(function))
		{}
		
		USE_RESULT std::basic_string<Char_t> ToString() const;
	};

	using SourceInfo = BasicSourceInfo<char>;
	using wSourceInfo = BasicSourceInfo<wchar_t>;
}


#define SOURCE_INFO_NO_FUNC klib::kDebug::SourceInfo(__FILE__, __LINE__, "")
#define SOURCE_INFO klib::kDebug::SourceInfo(__FILE__, __LINE__, __FUNCTION__)
#define SOURCE_INFO_VS klib::kDebug::SourceInfo(__FILE__, __LINE__, __FUNCSIG__)
#define WSOURCE_INFO klib::kDebug::wSourceInfo(__FILEW__, __LINE__, __FUNCTIONW__)
