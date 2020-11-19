#pragma once
#include "../kStringTypes.hpp"
#include "../Stringify/kStringIdentity.hpp"


namespace klib::kString::impl
{
	// Integral but not character types
	template<class Char_t, typename T>
	StringWriter<Char_t> Stringify(T arg, StringWriter<Char_t>& specifier)
	{
		const auto str = stringify::Identity<Char_t, T>::MakeStr(arg, specifier);
		return str;
	}
}
