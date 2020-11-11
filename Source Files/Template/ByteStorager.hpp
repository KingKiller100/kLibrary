#pragma once

#include "../HelperMacros.hpp"
#include "../Maths/BytesUnits.hpp"

#include <array>

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

		CONST_GETTER_CONSTEXPR(auto, GetTypeSize, TypeSize)
		CONST_GETTER_CONSTEXPR(auto, GetStorageType, StorageType)
		CONST_GETTER_CONSTEXPR(auto, GetStorageValue, StorageSize)
		CONST_GETTER_CONSTEXPR(auto, GetMaxBytes, MaxBytes)
		CONST_GETTER_CONSTEXPR(auto, GetLength, Length)
		CONST_GETTER_CONSTEXPR(auto, GetSizeInBytes, SizeInBytes)

		std::array<T, Length> storage;
	};
}
