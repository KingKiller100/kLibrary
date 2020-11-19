#include "pch.hpp"
#include "GenericSeeding.hpp"

#include "../../Utility/Debug/Exceptions/MathsExceptions.hpp"
#include "../../Utility/String/kToString.hpp"

#include <Windows.h>
#include <bcrypt.h>
#include <ctime>
#include <iostream>

namespace kmaths::kRng
{
	namespace
	{
		void LogBCryptError(const std::int64_t ret)
		{
			using namespace klib::kString;
			const auto binary =
				stringify::StringIntegralHex<char>(ret, 8);
			std::string error;
			switch (ret) {
			case 0xC0000225L:
				error = ToString("BCryptOpenAlgorithmProvider error, not found (0x{0})"
					, binary);
				break;

			case STATUS_INVALID_PARAMETER:
				error = ToString("BCryptOpenAlgorithmProvider error, invalid parameter (0x{0})"
					, binary);
				break;

			case STATUS_NO_MEMORY:
				error = ToString("BCryptOpenAlgorithmProvider error, no memory (0x{0})"
					, binary);
				break;

			default:
				error = ToString("BCryptOpenAlgorithmProvider error, other (0x{0})"
					, binary);
				break;
			}

			std::cerr << error << std::endl;
		}

		BCRYPT_ALG_HANDLE OpenCryptHandle()
		{
			BCRYPT_ALG_HANDLE algHandle;
			const auto ret = ::BCryptOpenAlgorithmProvider(&algHandle
				, BCRYPT_RNG_ALGORITHM
				, nullptr
				, 0ul);
			if (ret != 0)
				LogBCryptError(ret);
			return algHandle;
		}

		template<class Integral_t>
		Integral_t GenerateRandomNumber(BCRYPT_ALG_HANDLE algHandle)
		{
			static_assert(std::is_integral_v<Integral_t>, "Type Integral_t must be an integral type");

			constexpr auto bufferSize = std::numeric_limits<Integral_t>::digits;
			const auto buf = std::make_unique<BYTE[]>(bufferSize);
			const auto ret = ::BCryptGenRandom(algHandle, buf.get(), bufferSize, 0l);

			if (ret != 0)
			{
				LogBCryptError(ret);
				return static_cast<Integral_t>(-1);
			}

			const auto valuePtr = reinterpret_cast<Integral_t*>(buf.get());
			return *valuePtr;
		}

		void CloseBCryptHandle(BCRYPT_ALG_HANDLE algHandle)
		{
			const auto ret = ::BCryptCloseAlgorithmProvider(algHandle, 0ul);
			if (ret != 0)
				LogBCryptError(ret);
		}

		template<class Integral_t>
		Integral_t GetBCryptSeed()
		{
			const auto algHandle = OpenCryptHandle();
			const auto value = GenerateRandomNumber<Integral_t>(algHandle);
			CloseBCryptHandle(algHandle);
			return value;
		}

		std::time_t SeedFromTime()
		{
			return std::time(nullptr);
		}
	}

	std::uint64_t U64Seed(GenericSeedingSource sourceType)
	{
		switch (sourceType) {
		case GenericSeedingSource::TIME:
			return static_cast<std::uint64_t>(SeedFromTime());
		case GenericSeedingSource::BCRYPT:
			return GetBCryptSeed<std::uint64_t>();
		default:
			throw klib::kDebug::MathsError("Unknown seeding source type");
			break;
		}
	}

	std::uint32_t U32Seed(GenericSeedingSource sourceType)
	{
		switch (sourceType) {
		case GenericSeedingSource::TIME:
			return static_cast<std::uint32_t>(SeedFromTime());
		case GenericSeedingSource::BCRYPT:
			return GetBCryptSeed<std::uint32_t>();
		default:
			throw klib::kDebug::MathsError("Unknown seeding source type");
			break;
		}
	}
}

