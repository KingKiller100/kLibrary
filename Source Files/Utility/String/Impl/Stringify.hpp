#pragma once
#include "../kStringTypes.hpp"
#include "../Stringify/kStringIdentity.hpp"


namespace klib::kString::impl
{
	// Integral but not character types
	template<class Char_t, typename T>
	decltype(auto) Stringify(T arg, StringWriter<Char_t>& specifier)
	{
		return stringify::Identity<Char_t, T>::MakeStr(arg, specifier);
	}
}
