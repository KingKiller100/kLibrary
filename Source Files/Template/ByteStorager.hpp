#pragma once

#include "../HelperMacros.hpp"
#include "../Maths/BytesUnits.hpp"

namespace klib
{
	template<typename T, size_t N, kmaths::BytesUnits DigitalStorageType>
	struct BytesStorage
	{
		using Type = T;
		static constexpr auto TypeSize = sizeof(T);
		static constexpr auto StorageType = DigitalStorageType;
		static constexpr auto StorageSize = CAST(size_t, DigitalStorageType);
		static constexpr auto MaxBytes = StorageSize * N;
		static constexpr auto Length = MaxBytes / TypeSize;
		static constexpr auto SizeInBytes = Length * TypeSize;

		USE_RESULT constexpr const auto& GetTypeSize() const noexcept
		{
			return TypeSize;
		}
		USE_RESULT constexpr const auto& GetStorageType() const noexcept
		{
			return StorageType;
		}
		USE_RESULT constexpr const auto& GetStorageValue() const noexcept
		{
			return StorageSize;
		}
		USE_RESULT constexpr const auto& GetMaxBytes() const noexcept
		{
			return MaxBytes;
		}
		USE_RESULT constexpr const auto& GetLength() const noexcept
		{
			return Length;
		}
		USE_RESULT constexpr const auto& GetSizeInBytes() const noexcept
		{
			return SizeInBytes;
		}

		T storage[Length];
	};
}
