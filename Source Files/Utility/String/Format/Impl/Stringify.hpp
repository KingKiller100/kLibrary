#pragma once
#include "../../kStringTypes.hpp"
#include "../Stringify/kStringIdentity.hpp"


namespace klib::kString::stringify
{
	template<class Char_t, typename T>
	constexpr StringWriter<Char_t> Stringify(T arg, StringWriter<Char_t>& specifier)
	{
		const auto str = Identity<Char_t, T>::MakeStr(arg, specifier);
		return str;
	}
}
