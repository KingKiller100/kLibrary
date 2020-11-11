#pragma once

#include "kStringifyHelper.hpp"
#include "../kStringManipulation.hpp"

#include <cstddef>

namespace klib::kString::stringify
{
	template<class CharType>
	constexpr std::array<CharType, 16> s_GeneralHexMap = {
		static_cast<CharType>('0'), static_cast<CharType>('1'), static_cast<CharType>('2'), static_cast<CharType>('3'),
		static_cast<CharType>('4'), static_cast<CharType>('5'), static_cast<CharType>('6'), static_cast<CharType>('7'),
		static_cast<CharType>('8'), static_cast<CharType>('9'), static_cast<CharType>('a'), static_cast<CharType>('b'),
		static_cast<CharType>('c'), static_cast<CharType>('d'), static_cast<CharType>('e'), static_cast<CharType>('f'),
	};
	
	/// Digits of largest conceivable number for any integral type
	/// plus a null terminator + possible minus symbol
	template<class T, typename = std::enable_if_t<std::is_integral_v<T>>>
	constexpr std::size_t max_digits = std::numeric_limits<T>::digits10 + 2;

	template<class CharType, typename Uint_t
		, typename = std::enable_if_t<std::is_unsigned_v<Uint_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		constexpr CharType* UintToStr(CharType* current, Uint_t uVal)
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
		kString::StringWriter<CharType> StringSignedIntegral(const Signed_t val, size_t minDigits)
	{
		if (minDigits == nPrecision)
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
		PrependPadding(str, minDigits, CharType('0'));

		return str;
	}

	template<class CharType, typename Unsigned_t
		, typename = std::enable_if_t<std::is_integral_v<Unsigned_t>
		|| type_trait::Is_CharType_V<CharType>>
		>
		StringWriter<CharType> StringUnsignedIntegral(const Unsigned_t val, size_t minDigits)
	{
		if (minDigits == nPrecision)
			minDigits = 1;
		
		CharType buff[max_digits<Unsigned_t>]{};
		CharType* const end = std::end(buff);
		CharType* current = UintToStr(end, val);


		kString::StringWriter<CharType> str(current, end);
		PrependPadding(str, minDigits, CharType('0'));

		return str;
	}


	template<class CharType, typename Integral_t
		, typename = std::enable_if_t < std::is_integral_v<Integral_t>
	>>
		StringWriter<CharType> StringIntegral(const Integral_t val, size_t minDigits)
	{
		if constexpr (std::is_unsigned_v<Integral_t>)
			return StringUnsignedIntegral<CharType, Integral_t>(val, minDigits);
		else
			return StringSignedIntegral<CharType, Integral_t>(val, minDigits);
	}

	template<class CharType, typename Integral_t, typename = std::enable_if_t < 
		std::is_integral_v<Integral_t>
	>>
	StringWriter<CharType> StringIntegralHex(const Integral_t val, size_t padding)
	{
		static constexpr auto& hexMap = s_GeneralHexMap<CharType>;

		if (padding == nPrecision)
			padding = sizeof(uintptr_t) * 2;

		StringWriter<CharType> address;

		auto asUint = static_cast<size_t>(val);

		while (asUint > 0)
		{
			const auto index = asUint % hexMap.size();
			address.insert(address.begin(), hexMap.at(index));
			asUint /= hexMap.size();
		}

		PrependPadding(address, padding, CharType('0'));

		return address;
	}

	template<class CharType, typename Integral_t, typename = std::enable_if_t <
		std::is_integral_v<Integral_t>
		>>
		StringWriter<CharType> StringIntegralBinary(Integral_t val, size_t padding)
	{
		StringWriter<CharType> binary;
		binary.reserve(std::numeric_limits<Integral_t>::digits);
		
		while(val > 0)
		{
			const auto binVal = val % 2;
			binary.push_back(CharType('0') + binVal);
			val >>= 1;
		}

		return binary;
	}

	
}
