#pragma once

#include <cstdint>
#include <ratio>

namespace kmaths
{
	enum class BytesUnits : size_t
	{
		BYTE = sizeof(std::byte),
		KILO = std::kilo::num,
		KIBI = 1024,
		MEGA = std::mega::num,
		MEBI = KIBI * 1024,
		GIGA = std::giga::num,
		GIBI = MEBI * 1024,
		TERA = std::tera::num,
		TEBI = GIBI * 1024,
	};

	using Byte_Type = std::uint8_t;

	USE_RESULT constexpr size_t MinimumStorage(const size_t typeSize, const size_t count) noexcept
	{
		return typeSize * count;
	}

	template<typename T>
	USE_RESULT constexpr size_t MinimumStorage(const size_t count) noexcept
	{
		return sizeof(T) * count;
	}
	
}
