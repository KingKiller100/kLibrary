#pragma once

namespace klib::kTemplate
{
	template<typename T>
	constexpr auto GetTypeName()
	{
		return typeid(T).name();
	}
	
	template<typename T>
	constexpr auto GetTypeName(T&&)
	{
		return typeid(T).name();
	}
}
