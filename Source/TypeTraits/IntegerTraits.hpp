#pragma once

#include "TraitsBase.hpp"
#include "TemplateTraits.hpp"

#include <limits>

namespace klib::type_trait
{
	template<typename T>
	struct Traits<T, std::enable_if_t<Is_It_V<T,
	unsigned char
	, unsigned short
	, unsigned int
	, unsigned long
	, unsigned long long
	, signed char
	, short
	, int
	, long
	, long long
	>
	>>
	{
		using Type = T;
		using Limits_t = std::numeric_limits<T>;
		static constexpr auto Bytes = sizeof(Type);
		static constexpr auto Bits = Bytes * 8;
	};

	template<typename T>
	struct Is_IntegerBase : public std::bool_constant<Is_It_V<T,
		unsigned char
		, unsigned short
		, unsigned int
		, unsigned long
		, unsigned long long
		, signed char
		, short
		, int
		, long
		, long long
		>
	>
	{};

	template<typename T>
	struct Is_Integer : Is_IntegerBase<std::remove_cv_t<T>>
	{};

	template<typename T>
	constexpr auto Is_Integer_V = Is_Integer<T>::value;

#if MSVC_PLATFORM_TOOLSET > 141
	template<typename T>
	concept Is_Integer_c = Is_Integer_V<T> == true;
#endif
}
