#pragma once

#include "../HelperMacros.hpp"

#include <xtr1common>

namespace klib::type_trait
{
	template<bool Test>
	struct Compile_Time_Test // No class member value
	{}; 

	template<>
	struct Compile_Time_Test<true> : std::true_type // Has class member value
	{};
	
	template<bool Test>
	constexpr bool Compile_Time_Test_V = Compile_Time_Test<Test>::value;
}