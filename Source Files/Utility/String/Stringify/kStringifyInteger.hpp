#pragma once

#include "kStringifyHelper.hpp"
#include "../kStringTricks.hpp"

#include <cstddef>

namespace klib::kString::stringify
{
	/// Digits of largest conceivable number for any integral type
	/// plus a null terminator + possible minus symbol
	template<class T, typename = std::enable_if_t<std::is_integral_v<T>>>
	constexpr std::size_t max_digits = std::numeric_limits<T>::digits10 + 2;

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
		USE_RESULT constexpr StringWriter<CharType> StringSignedIntegral(Signed_t val, size_t minDigits = s_NoSpecifier, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		if (minDigits == s_NoSpecifier)
			minDigits = 1;
		using unsigned_t = std::make_unsigned_t<Signed_t>;
		CharType buff[max_digits<Signed_t>]{};
		CharType* const end = std::end(buff);
		CharType* current = end;
		const auto uVal = static_cast<unsigned_t>(val);

		if (val < 0)
		{
			current = UintToStr(current, 0 - uVal);
			*(--current) = CharType('-');
		}
		else
		{
			current = UintToStr(current, uVal);
		}

		kString::StringWriter<CharType> str(current, end);
		PrependPadding(str, minDigits, placeHolder);

		return str;
	}

	template<class CharType, typename Unsigned_t
		, typename = std::enable_if_t<std::is_integral_v<Unsigned_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		USE_RESULT constexpr StringWriter<CharType> StringUnsignedIntegral(Unsigned_t val, size_t minDigits = s_NoSpecifier, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		if (minDigits == s_NoSpecifier)
			minDigits = 1;

		CharType buff[max_digits<Unsigned_t>]{};
		CharType* const end = std::end(buff);
		CharType* current = UintToStr(end, val);


		kString::StringWriter<CharType> str(current, end);
		PrependPadding(str, minDigits, placeHolder);

		return str;
	}


	template<class CharType, typename Integral_t
		, typename = std::enable_if_t < std::is_integral_v<Integral_t>
		>>
		USE_RESULT constexpr StringWriter<CharType> StringIntegral(Integral_t val, size_t minDigits = s_NoSpecifier, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
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
		USE_RESULT StringWriter<CharType> StringIntegralHex(Integral_t val, size_t minCharacters = s_NoSpecifier, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		constexpr auto& hexMap = s_GeneralHexMap<CharType>;

		if (minCharacters == s_NoSpecifier)
			minCharacters = sizeof(uintptr_t) * 2;

		StringWriter<CharType> address;

		auto asUint = static_cast<size_t>(val);

		while (asUint > 0)
		{
			const auto index = asUint % hexMap.size();
			address.insert(address.begin(), hexMap.at(index));
			asUint /= hexMap.size();
		}
		
		if (val < 0 && address.front() == hexMap[15])
		{
			const auto notFpos = address.find_first_not_of(hexMap[15]);
			address = address.substr(notFpos - 1);
			if (placeHolder == s_DefaultPlaceHolder<CharType>)
				placeHolder = hexMap[15];
		}
		
		PrependPadding(address, minCharacters, placeHolder);

		return address;
	}

	// Big endian format
	template<class CharType, typename Integral_t, typename = std::enable_if_t <
		std::is_integral_v<Integral_t>
		>>
		USE_RESULT constexpr StringWriter<CharType> StringIntegralBinary(Integral_t val, size_t minCharacters, CharType placeHolder = s_DefaultPlaceHolder<CharType>)
	{
		StringWriter<CharType> binary;
		binary.reserve(std::numeric_limits<Integral_t>::digits);

		while (val > 0)
		{
			const auto binVal = val % 2;
			const CharType digit = static_cast<CharType>(CharType('0') + binVal);
			binary.push_back(digit);
			val >>= 1;
		}

		AppendPadding(binary, minCharacters, placeHolder);

		std::reverse(binary.begin(), binary.end());

		return binary;
	}
}


