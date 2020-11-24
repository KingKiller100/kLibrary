#pragma once

#include "kStringifyHelper.hpp"
#include "../../kStringTricks.hpp"

#include <cstddef>

namespace klib::kString::stringify
{
	enum class EndianFormat
	{
		BIG, LITTLE
	};

	/// Digits of largest conceivable number for any integral type
	/// plus a null terminator + possible minus symbol
	template<class T, typename = std::enable_if_t<std::is_integral_v<T>>>
	constexpr std::size_t s_MaxDigits = std::numeric_limits<T>::digits10 + 2;

	/// Bit count of largest conceivable number for any integral type
	/// plus a null terminator
	template<class T, typename = std::enable_if_t<std::is_integral_v<T>>>
	constexpr std::size_t s_MaxBits = std::numeric_limits<T>::digits + 1;

	template<class CharType, typename Uint_t
		, typename = std::enable_if_t<std::is_unsigned_v<Uint_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		USE_RESULT constexpr CharType* UintToStr(CharType* current, Uint_t uVal)
	{
		do {
			const auto mod = uVal % 10;
			*(--current) = static_cast<CharType>('0' + mod);
			uVal /= 10;
		} while (uVal != 0);
		return current;
	}

	template<class CharType, typename Signed_t
		, typename = std::enable_if_t<std::is_integral_v<Signed_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		USE_RESULT constexpr StringWriter<CharType> StringSignedIntegral(Signed_t val, size_t minDigits = s_NoSpecifier
			, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		if (minDigits == s_NoSpecifier)
			minDigits = 1;

		using Unsigned_t = std::make_unsigned_t<Signed_t>;
		CharType buff[s_MaxDigits<Signed_t>]{ type_trait::s_NullTerminator<CharType> };
		CharType* const end = std::end(buff) - 1;
		CharType* current = end;
		const auto uVal = static_cast<Unsigned_t>(val);

		const auto isNeg = val < 0;

		if (isNeg)
			current = UintToStr(current, 0 - uVal);
		else
			current = UintToStr(current, uVal);


		if (minDigits < s_MaxDigits<Signed_t>)
			PrependPadding(current, minDigits, placeHolder);

		if (isNeg)
			*(--current) = CharType('-');

		return StringWriter<CharType>(current, end);
	}

	template<class CharType, typename Unsigned_t
		, typename = std::enable_if_t<std::is_integral_v<Unsigned_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		USE_RESULT constexpr StringWriter<CharType> StringUnsignedIntegral(Unsigned_t val, size_t minDigits = s_NoSpecifier
			, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		if (minDigits == s_NoSpecifier)
			minDigits = 1;

		CharType buff[s_MaxDigits<Unsigned_t>]{ type_trait::s_NullTerminator<CharType> };
		CharType* const end = std::end(buff) - 1;
		CharType* current = UintToStr(end, val);

		if (minDigits < s_MaxDigits<Unsigned_t>)
			PrependPadding(current, minDigits, placeHolder);

		return StringWriter<CharType> (current, end);
	}


	template<class CharType, typename Integral_t
		, typename = std::enable_if_t < std::is_integral_v<Integral_t>
		>>
		USE_RESULT constexpr StringWriter<CharType> StringIntegral(Integral_t val, size_t minDigits = s_NoSpecifier
			, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		if constexpr (std::is_unsigned_v<Integral_t>)
			return StringUnsignedIntegral<CharType, Integral_t>(val, minDigits, placeHolder);
		else
			return StringSignedIntegral<CharType, Integral_t>(val, minDigits, placeHolder);
	}

	// Big endian format
	template<class CharType, typename Integral_t, typename = std::enable_if_t <
		std::is_integral_v<Integral_t>
		>>
		USE_RESULT StringWriter<CharType> StringIntegralHex(Integral_t val, size_t minCharacters = s_NoSpecifier
			, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		constexpr auto& hexMap = s_GeneralHexMap<CharType>;

		if (minCharacters == s_NoSpecifier)
			minCharacters = sizeof(uintptr_t) * 2;

		using Unsigned_t = std::make_unsigned_t<Integral_t>;

		CharType buff[s_MaxDigits<size_t>]{ type_trait::s_NullTerminator<CharType> };
		CharType* const end = std::end(buff) - 1;
		CharType* current = end;
		auto asUint = static_cast<Unsigned_t>(val);

		while (asUint > 0)
		{
			const auto index = asUint % hexMap.size();
			*(--current) = hexMap[index];
			asUint /= static_cast<Unsigned_t>(hexMap.size());
		}

		if (val < 0 && current[0] == hexMap[15])
		{
			const auto notFpos = Find_First_Not_Of(current, hexMap[15]);
			current += (notFpos - 1);
			if (placeHolder == s_DefaultPlaceHolder<CharType>)
				placeHolder = hexMap[15];
		}

		PrependPadding(current, minCharacters, placeHolder);

		return StringWriter<CharType>(current, end);
	}

	template<class CharType, typename Integral_t, typename = std::enable_if_t <
		std::is_integral_v<Integral_t>
		>>
		USE_RESULT constexpr StringWriter<CharType> StringIntegralBinary(Integral_t val, size_t minCharacters
			, CharType placeHolder = s_DefaultPlaceHolder<CharType>, EndianFormat endian = EndianFormat::BIG)
	{
		if (minCharacters == s_NoSpecifier)
			minCharacters = 1;

		using Unsigned_t = std::make_unsigned_t<Integral_t>;

		CharType buff[s_MaxBits<Integral_t>]{ type_trait::s_NullTerminator<CharType> };
		CharType* const end = std::end(buff) - 1;
		CharType* current = end;
		
		while (val > 0)
		{
			const auto binVal = val % 2;
			const CharType digit = static_cast<CharType>('0' + binVal);
			*(--current) = digit;
			val >>= 1;
		}

		PrependPadding(current, minCharacters, placeHolder);

		if (endian == EndianFormat::LITTLE)
			std::reverse(current, end);

		return StringWriter<CharType>(current, end);
	}
}


