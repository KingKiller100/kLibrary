#pragma once

#include "kStringifyInteger.hpp"
#include "../../../Type Traits/StringTraits.hpp"
#include "../../String/kStringTypes.hpp"

namespace klib::kString::stringify
{

	
	template<class CharType, typename T
		, typename = std::enable_if_t<std::is_pointer_v<T>
		|| type_trait::Is_CharType_V<CharType>>
		>
		StringWriter<CharType> StringifyPointer(const T* ptr, size_t padding)
	{
		const auto asUint = reinterpret_cast<uintptr_t>(ptr);
		const auto address =
			StringIntegralHex<CharType>(asUint, padding);
		return address;
	}
}
