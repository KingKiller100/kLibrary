#pragma once

#include "kStringifyInteger.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "../../String/kStringTypes.hpp"

#include "../Type/kStringExtract.hpp"

namespace klib::kString::stringify
{
	template<class CharType, typename T
		, typename = std::enable_if_t<std::is_pointer_v<T>
		|| type_trait::Is_CharType_V<CharType>>
		>
		StringWriter<CharType> StringifyPointer(const T* ptr, StringWriter<CharType>& specifier)
	{
		constexpr auto defaultPadding = sizeof(intptr_t) * 2;

		const auto binaryMode = Remove(specifier, kString::impl::s_BinaryModeToken<CharType>);
		
		const auto padding = StrTo<size_t>(specifier, defaultPadding);
		const auto asUint = reinterpret_cast<uintptr_t>(ptr);

		StringWriter<CharType> address;
		if (binaryMode)
			address = StringIntegral<CharType>(asUint, padding);
		else
			address = StringIntegralHex<CharType>(asUint, padding);
		
		return address;
	}
	
}
