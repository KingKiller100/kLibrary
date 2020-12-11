#pragma once

#include "kStringifyHelper.hpp"
#include "../../kStringTricks.hpp"
#include "../../../Maths/kCount.hpp"

#include <cstddef>

namespace klib::kString::stringify
{
	enum class EndianFormat
	{
		BIG, LITTLE
	};

	template<class Char_t, typename Uint_t, typename = std::enable_if_t<
		std::is_unsigned_v<Uint_t> || type_trait::Is_CharType_V<Char_t>>
		>
		USE_RESULT constexpr Char_t* UintToStr(Char_t* outHead, Uint_t uVal, const unsigned base = 10)
	{
		do {
			const auto index = uVal % static_cast<Uint_t>(base);
			*(--outHead) = g_GeneralAlphaMap<Char_t>[index];
			uVal /= static_cast<Uint_t>(base);
		} while (uVal != 0);
		return outHead;
	}

	template<class Char_t, typename Signed_t, typename = std::enable_if_t<
		std::is_integral_v<Signed_t> || type_trait::Is_CharType_V<Char_t>>
		>
		USE_RESULT constexpr const Char_t* StringSignedIntegral(Signed_t val, size_t minDigits = s_NoSpecifier
			, Char_t placeHolder = s_DefaultPlaceHolder<Char_t>, const unsigned base = 10)
	{
		using Unsigned_t = std::make_unsigned_t<Signed_t>;

		if (minDigits == s_NoSpecifier)
			minDigits = 1;
		
		const auto isNeg = kmaths::IsNegative(val);
		size_t size = kmaths::CountIntegerDigits(val);
		if (size < minDigits)
		{
			const auto extra = minDigits - size;
			size += extra;
		}
		if (isNeg)
			++size;
		
		Char_t* buff = new Char_t[size + 1]{ };
		Char_t* const end = buff + size;
		Char_t* current = end;
		const auto uVal = isNeg
			? 0 - static_cast<Unsigned_t>(val)
			: static_cast<Unsigned_t>(val);

		current = UintToStr(current, uVal, base);

		if (minDigits != s_NoSpecifier && minDigits < g_MaxDigits<Signed_t>)
			PrependPadding(current, minDigits, placeHolder);

		if (isNeg)
			PrependMinusSign(current);

		return (const Char_t*)buff;
	}

	template<class Char_t, typename Unsigned_t, typename = std::enable_if_t<
		std::is_integral_v<Unsigned_t> || type_trait::Is_CharType_V<Char_t>>
		>
		USE_RESULT constexpr const Char_t* StringUnsignedIntegral(Unsigned_t val, size_t minDigits = s_NoSpecifier
			, Char_t placeHolder = s_DefaultPlaceHolder<Char_t>, const unsigned base = 10)
	{
		if (minDigits == s_NoSpecifier)
			minDigits = 1;

		size_t size = kmaths::CountIntegerDigits(val);
		if (size < minDigits)
		{
			const auto extra = minDigits - size;
			size += extra;
		}
		Char_t* buff = new Char_t[size + 1]{ };
		Char_t* const end = buff + size;
		Char_t* current = UintToStr(end, val, base);

		if (minDigits < g_MaxDigits<Unsigned_t>)
			PrependPadding(current, minDigits, placeHolder);

		return (const Char_t*)buff;
	}

	template<class Char_t, typename Integral_t, typename = std::enable_if_t<
		std::is_integral_v<Integral_t>
		>>
		USE_RESULT constexpr const Char_t* StringIntegral(Integral_t val, size_t minDigits = s_NoSpecifier
			, Char_t placeHolder = s_DefaultPlaceHolder<Char_t>, const unsigned base = 10)
	{
		if constexpr (std::is_unsigned_v<Integral_t>)
			return StringUnsignedIntegral<Char_t, Integral_t>(val, minDigits, placeHolder, base);
		else
			return StringSignedIntegral<Char_t, Integral_t>(val, minDigits, placeHolder, base);
	}

	// Big endian format
	template<class Char_t, typename Integral_t, typename = std::enable_if_t<
		std::is_integral_v<Integral_t>
		>>
		USE_RESULT const Char_t* StringIntegralHex(Integral_t val, size_t minCharacters = s_NoSpecifier
			, Char_t placeHolder = s_DefaultPlaceHolder<Char_t>)
	{
		constexpr auto& hexMap = g_GeneralHexMap<Char_t>;

		if (minCharacters == s_NoSpecifier)
			minCharacters = sizeof(uintptr_t) * 2;

		if (val < 0 && placeHolder == s_DefaultPlaceHolder<Char_t>)
			placeHolder = hexMap[15];

		using Unsigned_t = std::make_unsigned_t<Integral_t>;

		Char_t buff[g_MaxDigits<size_t>]{ type_trait::g_NullTerminator<Char_t> };
		Char_t* const end = std::end(buff) - 1;
		Char_t* current = end;
		auto asUint = static_cast<Unsigned_t>(val);

		while (asUint > 0)
		{
			const auto index = asUint % hexMap.size();
			*(--current) = hexMap[index];
			asUint /= static_cast<Unsigned_t>(hexMap.size());
		}

		if (const auto size = (end - current);
			static_cast<size_t>(size) > minCharacters)
		{
			const auto notFPos = Find_First_Not_Of(current, hexMap[15]);
			const auto difference =
				static_cast<intptr_t>(minCharacters) - static_cast<intptr_t>(notFPos);
			if (difference > 0)
				current += difference;
		}
		else
		{
			PrependPadding(current, minCharacters, placeHolder);
		}

		return CreateNewCString(current);
	}

	template<class Char_t, typename Integral_t, typename = std::enable_if_t<
		std::is_integral_v<Integral_t>
		>>
		USE_RESULT constexpr const Char_t* StringIntegralBinary(Integral_t val, size_t minCharacters
			, Char_t placeHolder = s_DefaultPlaceHolder<Char_t>, EndianFormat endian = EndianFormat::BIG)
	{
		if (minCharacters == s_NoSpecifier)
			minCharacters = 1;

		Char_t buff[g_MaxBits<Integral_t>]{ type_trait::g_NullTerminator<Char_t> };
		Char_t* const end = std::end(buff) - 1;
		Char_t* current = end;

		do {
			const auto binVal = val % 2;
			const Char_t digit = static_cast<Char_t>('0' + binVal);
			*(--current) = digit;
			val >>= 1;
		} while (val > 0);

		PrependPadding(current, minCharacters, placeHolder);

		if (endian == EndianFormat::LITTLE)
			std::reverse(current, end);

		return CreateNewCString(current);
	}
}


