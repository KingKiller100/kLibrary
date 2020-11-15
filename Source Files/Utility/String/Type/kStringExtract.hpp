#pragma once

#include "../kStringTypes.hpp"
#include "../Stringify/kStringifyHelper.hpp"

#include "../Stringify/kStringifyFloatingPoint.hpp"
#include "../Stringify/kStringifyInteger.hpp"

#include <any>

namespace klib::kString::type
{
	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_BinaryModeToken = CharType('b');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_HexModeToken = CharType('h');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_GeneralFloatModeToken = CharType('g');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_FixedFloatModeToken = CharType('f');

	template<class CharType, typename = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr CharType s_ScientificFloatModeToken = CharType('e');

	template<typename T>
	decltype(auto) ExtractPtrFromAny(const std::any& container)
	{
		return std::any_cast<T*>(container);
	}

	template<typename CharT, typename T>
	void ExtractIntegerAndInsertInOutput(const std::any& container, StringWriter<CharT>& specifier, StringWriter<CharT>& outCurrentSection)
	{
		auto data = ExtractPtrFromAny<T>(container);
		const auto hexMode = Remove(specifier, s_HexModeToken<CharT>);
		const auto binaryMode = Remove(specifier, s_BinaryModeToken<CharT>);
		const auto padding = StrTo<size_t>(specifier, stringify::s_NoSpecifier);

		StringWriter<CharT> toAppend;

		if (hexMode)
			toAppend = stringify::StringIntegralHex<CharT>(*data, padding, CharT('0'));
		else if (binaryMode)
			toAppend = stringify::StringIntegralBinary<CharT, ONLY_TYPE(T)>(*data, padding, CharT('0'));
		else
			toAppend = stringify::StringIntegral<CharT, ONLY_TYPE(T)>(*data, padding, CharT('0'));

		outCurrentSection.append(toAppend);
	}

	template<typename CharT>
	void ExtractCharAndInsertInOutput(const std::any& container, [[maybe_unused]] const StringWriter<CharT>& specifier
		, StringWriter<CharT>& outCurrentSection)
	{
		auto data = ExtractPtrFromAny<CharT>(container);
		outCurrentSection.push_back(*data);
	}

	template<typename StringT>
	void ExtractStringAndInsertInOutput(const std::any& container, [[maybe_unused]] const StringWriter<typename StringT::value_type>& specifier
		, StringWriter<typename StringT::value_type>& outCurrentSection)
	{
		const auto size = StrTo<long long>(specifier, StringT::npos);
		auto data = ExtractPtrFromAny<StringT>(container);
		outCurrentSection.append(*data);
	}

	template<typename CharT, typename T>
	void ExtractFloatAndInsertInOutput(const std::any& container, StringWriter<CharT>& specifier
		, StringWriter<CharT>& outCurrentSection)
	{
		StringWriter<CharT> toAppend;
		auto data = ExtractPtrFromAny<T>(container);

		std::chars_format fmt = std::chars_format::general;
		if (Remove(specifier, s_ScientificFloatModeToken<CharT>))
		{
			fmt = std::chars_format::scientific;
		}
		else if (Remove(specifier, s_HexModeToken<CharT>))
		{
			fmt = std::chars_format::hex;
		}
		else if (Remove(specifier, s_FixedFloatModeToken<CharT>))
		{
			fmt = std::chars_format::fixed;
		}

		const auto binaryMode = Remove(specifier, s_BinaryModeToken<CharT>);

		const auto padding = StrTo<std::int64_t>(specifier, stringify::s_NoSpecifier);
		
		if (binaryMode)
			toAppend = stringify::StringIntegralBinary<CharT, size_t>(*reinterpret_cast<const size_t*>(data), padding, CharT('0'));
		else
			toAppend = stringify::StringFloatingPoint<CharT>(*data, padding, fmt);

		outCurrentSection.append(toAppend);
	}

	template<typename CharT>
	void ExtractBoolAndInsertInOutput(const std::any& container, StringWriter<CharT>& specifier
		, StringWriter<CharT>& outCurrentSection)
	{
		const auto data = ExtractPtrFromAny<const bool>(container);

		union DummyUnion
		{
			bool real;
			size_t uint;
		} dummy;
		
		dummy.real = *data;

		const auto hexMode = Remove(specifier, s_HexModeToken<CharT>);
		const auto binaryMode = Remove(specifier, s_BinaryModeToken<CharT>);
		const auto padding = StrTo<size_t>(specifier, stringify::s_NoSpecifier);

		StringWriter<CharT> toAppend;

		if (hexMode)
			toAppend = stringify::StringIntegralHex<CharT>(dummy.uint, padding, CharT('0'));
		else if (binaryMode)
			toAppend = stringify::StringIntegralBinary<CharT, size_t>(dummy.uint, padding, CharT('0'));
		else
			toAppend = stringify::StringBool<CharT>(dummy.real);

		outCurrentSection.append(toAppend);
	}
}
