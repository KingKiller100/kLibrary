#pragma once

#include "../../kStringTypes.hpp"
#include "../Stringify/kStringifyHelper.hpp"

#include "../Stringify/kStringifyFloatingPoint.hpp"
#include "../Stringify/kStringifyInteger.hpp"
#include "../Stringify/kStringifyBool.hpp"


namespace klib::kString::stringify
{
	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_BinaryModeToken = CharType('b');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_HexModeToken = CharType('h');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_BoolAsInt = CharType('d');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_GeneralFloatModeToken = CharType('g');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_FixedFloatModeToken = CharType('f');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_ScientificFloatModeToken = CharType('e');

	template<typename CharT, typename T>
	StringWriter<CharT> HandleInteger(T value, StringWriter<CharT>& specifier)
	{
		const auto hexMode = Remove(specifier, s_HexModeToken<CharT>);
		const auto binaryMode = Remove(specifier, s_BinaryModeToken<CharT>);
		const auto padding = StrTo<size_t>(specifier, stringify::s_NoSpecifier);

		StringWriter<CharT> str;

		if (hexMode)
			str = stringify::StringIntegralHex<CharT>(value, padding, CharT('0'));
		else if (binaryMode)
			str = stringify::StringIntegralBinary<CharT, ONLY_TYPE(T)>(value, padding, CharT('0'));
		else
			str = stringify::StringIntegral<CharT, ONLY_TYPE(T)>(value, padding, CharT('0'));
		return str;
	}

	template<typename CharT>
	decltype(auto) HandleCharacter(const CharT value, UNUSED const StringWriter<CharT>& specifier)
	{
		return value;
	}

	template<typename CharT>
	StringWriter<CharT> HandleCharPointer(const CharT* value, UNUSED const StringWriter<CharT>& specifier)
	{
		return value;
	}

	template<typename CharT, typename T>
	decltype(auto) HandlePointer(T* ptr, StringWriter<CharT>& specifier)
	{
		constexpr auto defaultPadding = sizeof(intptr_t) * 2;

		const auto binaryMode = Remove(specifier, s_BinaryModeToken<CharT>);

		const auto padding = StrTo<size_t>(specifier, defaultPadding);
		auto asUint = reinterpret_cast<uintptr_t>(ptr);

		StringWriter<CharT> address;
		if (binaryMode)
			address = stringify::StringIntegral<CharT>(asUint, padding);
		else
			address = stringify::StringIntegralHex<CharT>(asUint, padding);
		return address;
	}

	template<typename StringT>
	const StringT& HandleSTLString(const StringT& value, UNUSED const StringWriter<typename StringT::value_type>& specifier)
	{
		return value;
	}

	template<typename CharT, typename T>
	StringWriter<CharT> HandleFloat(T value, StringWriter<CharT>& specifier)
	{
		using IntegralSizeMatch_t = std::conditional_t<sizeof(T) == 4, std::int32_t, std::int64_t>;
		
		StringWriter<CharT> str;

		std::chars_format fmt = std::chars_format::fixed;
		if (Remove(specifier, s_ScientificFloatModeToken<CharT>))
		{
			fmt = std::chars_format::scientific;
		}
		else if (Remove(specifier, s_HexModeToken<CharT>))
		{
			fmt = std::chars_format::hex;
		}
		/*else if (Remove(specifier, s_FixedFloatModeToken<CharT>))
		{
			fmt = std::chars_format::fixed;
		}*/
		else if (Remove(specifier, s_GeneralFloatModeToken<CharT>))
		{
			fmt = std::chars_format::general;
		}

		const auto binaryMode = Remove(specifier, s_BinaryModeToken<CharT>);

		const auto padding = StrTo<std::int64_t>(specifier, stringify::s_NoSpecifier);

		if (binaryMode)
			str = stringify::StringIntegralBinary<CharT, size_t>(*(IntegralSizeMatch_t*)&value, padding, CharT('0'));
		else
			str = stringify::StringFloatingPoint<CharT>(value, padding, fmt);
		return str;
	}

	template<typename CharT>
	StringWriter<CharT> HandleBool(bool value, StringWriter<CharT>& specifier)
	{
		union DummyUnion
		{
			bool real;
			uint8_t uint;
		} dummy;

		dummy.real = value;

		const auto hexMode = Remove(specifier, s_HexModeToken<CharT>);
		const auto binaryMode = Remove(specifier, s_BinaryModeToken<CharT>);
		const auto basiMode = Remove(specifier, s_BoolAsInt<CharT>);
		const auto padding = StrTo<size_t>(specifier, stringify::s_NoSpecifier);

		StringWriter<CharT> str;

		if (hexMode)
			str = stringify::StringIntegralHex<CharT>(dummy.uint, padding, CharT('0'));
		else if (binaryMode)
			str = stringify::StringIntegralBinary<CharT>(dummy.uint, padding, CharT('0'));
		else if (basiMode)
			str = stringify::StringIntegral<CharT>(dummy.uint, padding, CharT('0'));
		else
			str = stringify::StringBool<CharT>(dummy.real);
		return str;
	}
}
