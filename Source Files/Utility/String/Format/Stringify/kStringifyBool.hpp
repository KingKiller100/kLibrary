﻿#pragma once

#include "../../../../TypeTraits/StringTraits.hpp"

namespace klib::kString::stringify
{
	template<class Char_t
		, typename = std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>>
		>
	const Char_t* StringBool(bool val)
	{
		Char_t buff[6]{ type_trait::g_NullTerminator<Char_t> };
		Char_t* const end = std::end(buff) - 1;
		Char_t* current = end;

		if (val)
		{
			*(--current) = Char_t('e');
			*(--current) = Char_t('u');
			*(--current) = Char_t('r');
			*(--current) = Char_t('t');
		}
		else
		{
			*(--current) = Char_t('e');
			*(--current) = Char_t('s');
			*(--current) = Char_t('l');
			*(--current) = Char_t('a');
			*(--current) = Char_t('f');
		}
		
		const auto cstr = CreateNewPointer(current);
		return std::move(cstr);
	}
	
}
