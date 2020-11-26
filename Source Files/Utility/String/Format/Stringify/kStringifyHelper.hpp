﻿#pragma once

#include "../../../../TypeTraits/StringTraits.hpp"
#include "../../kStringTricks.hpp"

namespace klib::kString::stringify
{
	constexpr auto s_NoSpecifier = static_cast<size_t>(-1);

	template<class Char_t, typename = std::enable_if_t<type_trait::Is_CharType_V<Char_t>>>
	constexpr auto s_DefaultPlaceHolder = Char_t('0');

	template<class Char_t>
	constexpr std::array<Char_t, 16> g_GeneralHexMap = {
		static_cast<Char_t>('0'), static_cast<Char_t>('1'), static_cast<Char_t>('2'), static_cast<Char_t>('3'),
		static_cast<Char_t>('4'), static_cast<Char_t>('5'), static_cast<Char_t>('6'), static_cast<Char_t>('7'),
		static_cast<Char_t>('8'), static_cast<Char_t>('9'), static_cast<Char_t>('a'), static_cast<Char_t>('b'),
		static_cast<Char_t>('c'), static_cast<Char_t>('d'), static_cast<Char_t>('e'), static_cast<Char_t>('f'),
	};

	/// Digits of largest conceivable number for any integral type
	/// plus a null terminator + possible minus symbol
	template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
	constexpr size_t g_MaxDigits = std::numeric_limits<T>::digits10 + 2;

	/// Digits of largest conceivable number for any floating point type
	/// plus a null terminator + possible minus symbol
	template<class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr size_t g_MaxFloatDigits = std::numeric_limits<T>::max_digits10 + 2;

	/// Bit count of largest conceivable number for any integral type
	/// plus a null terminator
	template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
	constexpr size_t g_MaxBits = std::numeric_limits<T>::digits + 1;
	
	template<typename CharType>
	void PrependPadding(std::basic_string<CharType>& outStr, const size_t minDigits, CharType padding)
	{
		if (outStr.size() >= minDigits)
			return;

		const auto count = minDigits - outStr.size();
		outStr.insert(outStr.begin(), count, padding);
	}

	template<typename CharType>
	void PrependPadding(CharType*& outHeadOfPtr, const size_t minDigits, CharType padding)
	{
		const auto size = GetSize(outHeadOfPtr);

		if (size >= minDigits)
			return;

		auto count = minDigits - size;
		while (count > 0)
		{
			*(--outHeadOfPtr) = padding;
			--count;
		}
	}

	template<typename CharType>
	void AppendPadding(std::basic_string<CharType>& outStr, const size_t minDigits, CharType padding)
	{
		if (outStr.size() >= minDigits)
			return;

		const auto count = minDigits - outStr.size();
		outStr.insert(outStr.end(), count, padding);
	}

	template<class Char_t>
	const Char_t* CreateNewPointer(const Char_t* ptr)
	{
		const auto size = GetSize(ptr) + 1;
		auto cstr = new Char_t[size]();
		const auto bytes = size * sizeof (Char_t);
		std::memcpy(cstr, ptr, bytes);
		return (const Char_t*)cstr;
	}
}
