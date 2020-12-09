#pragma once

#include "../../kStringTypes.hpp"
#include "../Stringify/kStringifyHelper.hpp"

#include "../Stringify/kStringifyFloat.hpp"
#include "../Stringify/kStringifyInteger.hpp"
#include "../Stringify/kStringifyBool.hpp"


namespace klib::kString::stringify
{
	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType g_BinaryModeToken = CharType('b');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType g_HexModeToken = CharType('h');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType g_AsNumber = CharType('d');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType g_GeneralFloatModeToken = CharType('g');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType g_FixedFloatModeToken = CharType('f');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType g_ScientificFloatModeToken = CharType('e');

	template<typename CharT, typename T>
	constexpr const CharT* HandleInteger(T value, StringWriter<CharT>& specifier)
	{
		const auto hexMode = Remove(specifier, g_HexModeToken<CharT>);
		const auto binaryMode = Remove(specifier, g_BinaryModeToken<CharT>);
		const auto padding = CStrTo<size_t>(specifier.data(), stringify::s_NoSpecifier);

		if (hexMode)
			return stringify::StringIntegralHex<CharT>(value, padding, CharT('0'));
		if (binaryMode)
			return stringify::StringIntegralBinary<CharT, ONLY_TYPE(T)>(value, padding, CharT('0'));

		return stringify::StringIntegral<CharT, ONLY_TYPE(T)>(value, padding, CharT('0'));
	}

	template<typename CharT>
	decltype(auto) HandleCharacter(const CharT value, UNUSED const StringWriter<CharT>& specifier)
	{
		return value;
	}

	template<typename CharT>
	std::basic_string<CharT> HandleCharPointer(const CharT* value, UNUSED const StringWriter<CharT>& specifier)
	{
		return value;
	}

	template<typename CharT, typename T>
	decltype(auto) HandlePointer(T* ptr, StringWriter<CharT>& specifier)
	{
		const auto asUint = reinterpret_cast<uintptr_t>(ptr);

		const auto numberMode = Remove(specifier, g_AsNumber<CharT>);
		const auto binaryMode = Remove(specifier, g_BinaryModeToken<CharT>);
		const auto padding = CStrTo<size_t>(specifier.data(), stringify::s_NoSpecifier);

		if (binaryMode)
			return stringify::StringIntegralBinary<CharT>(asUint, padding);

		if (numberMode)
			return stringify::StringIntegral<CharT>(asUint, padding);

		return stringify::StringIntegralHex<CharT>(asUint, padding);
	}

	template<typename StringT>
	const StringT& HandleSTLString(const StringT& value, UNUSED const StringWriter<typename StringT::value_type>& specifier)
	{
		return value;
	}

	template<typename CharT, typename T>
	constexpr const CharT* HandleFloat(T value, StringWriter<CharT>& specifier)
	{
		FloatFormat fmt = FloatFormat::FIX;

		if (Remove(specifier, g_ScientificFloatModeToken<CharT>))
		{
			fmt = FloatFormat::SCI;
		}
		else if (Remove(specifier, g_GeneralFloatModeToken<CharT>))
		{
			fmt = FloatFormat::GEN;
		}
		else if (Remove(specifier, g_HexModeToken<CharT>))
		{
			fmt = FloatFormat::HEX;
		}
		else if (Remove(specifier, g_BinaryModeToken<CharT>))
		{
			fmt = FloatFormat::BIN;
		}

		const auto padding = CStrTo<size_t>(specifier.data(), stringify::s_NoSpecifier);
		return stringify::StringFloatingPoint<CharT>(value, padding, fmt);
	}

	template<typename CharT>
	constexpr const CharT* HandleBool(bool value, StringWriter<CharT>& specifier)
	{
		union DummyUnion
		{
			bool real;
			uint8_t uint;
		} dummy;

		dummy.real = value;

		const auto hexMode = Remove(specifier, g_HexModeToken<CharT>);
		const auto numberMode = Remove(specifier, g_AsNumber<CharT>);
		const auto binaryMode = Remove(specifier, g_BinaryModeToken<CharT>);
		const auto padding = CStrTo<size_t>(specifier.data(), stringify::s_NoSpecifier);

		if (numberMode)
			return stringify::StringIntegral<CharT>(dummy.uint, padding, CharT('0'));
		if (hexMode)
			return stringify::StringIntegralHex<CharT>(dummy.uint, padding, CharT('0'));
		if (binaryMode)
			return stringify::StringIntegralBinary<CharT>(dummy.uint, padding, CharT('0'));

		return stringify::StringBool<CharT>(dummy.real);
	}
}
