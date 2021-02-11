#pragma once

namespace klib::type_trait
{
	template<typename T, typename Requirement_t = void>
	struct Traits
	{
		static constexpr auto Bytes = sizeof(T);
		static constexpr auto Bits = Bytes * 8;
	};
}
