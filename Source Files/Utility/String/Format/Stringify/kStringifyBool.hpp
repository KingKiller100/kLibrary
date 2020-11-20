#pragma once

#include "../../../../TypeTraits/StringTraits.hpp"
#include "../../../Debug/Exceptions/StringExceptions.hpp"

namespace klib::kString::stringify
{
	template<class CharType, typename T
		, typename = std::enable_if_t<std::is_same_v<T, bool>
		|| type_trait::Is_CharType_V<CharType>>
		>
	kString::StringWriter<CharType> StringBool(const T val)
	{
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
			return val ? "true" : "false";
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
			return val ? L"true" : L"false";
		else if _CONSTEXPR_IF(std::is_same_v<CharType, char16_t>)
			return val ? u"true" : u"false";
		else if _CONSTEXPR_IF(std::is_same_v<CharType, char32_t>)
			return val ? U"true" : U"false";
#ifdef __cpp_char8_t
		else if _CONSTEXPR_IF(std::is_same_v<CharType, char8_t>)
			return  val ? u8"true" : u8"false";
#endif

		const std::string type = typeid(T).name();
		throw kDebug::FormatError("Cannot stringify bool to unknown char type: " + type);
	}
	
}
