#pragma once

#include "../../../../TypeTraits/StringTraits.hpp"
#include "../../../Debug/Exceptions/StringExceptions.hpp"

namespace klib::kString::stringify
{
	template<class CharType
		, typename = std::enable_if_t<
		type_trait::Is_CharType_V<CharType>>
		>
	decltype(auto) StringBool(bool val)
	{
		const auto size = val ? 5 : 6;
		auto buf = std::make_unique<CharType[]>(size);
		buf[size - 1] = CharType('\0');

		const auto res = val ? "true" : "false";
		const auto temp = Convert<CharType>(res);
		memcpy(buf.get(), temp, size);

		return std::move(buf);
	}
	
}
