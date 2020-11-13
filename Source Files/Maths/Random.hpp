#pragma once

#include "../HelperMacros.hpp"

#include <random>
#include <ctime>

namespace kmaths
{
	namespace kRng
	{
		enum class RngSeedSource
		{
			TIME,
			BCRYPT,
		};

		std::time_t SeedFromTime();
		std::uint64_t UInt64FromThermalNoise();

		/**
		 * \brief
		 *		Random number generator
		 * \tparam Primitive_t
		 *		Return type and param type
		 * \param lowerBound
		 *		Lower bound
		 * \param upperBound
		 *		Upper bound
		 * \param seedType
		 *		Rng seed seedType
		 * \return
		 *		Random number between the given bounds
		 */
		template<typename Primitive_t>
		USE_RESULT static Primitive_t RNG(Primitive_t lowerBound, Primitive_t upperBound, RngSeedSource seedType = RngSeedSource::BCRYPT)
		{
			static_assert(std::is_arithmetic_v<Primitive_t>, "Must be an arithmetic type");

			static auto currentSeedType = seedType;
			static std::mt19937_64 generator;

			switch (seedType) {
			case RngSeedSource::TIME:
				if (seedType != currentSeedType)
					generator.seed(SeedFromTime());
				break;
			case RngSeedSource::BCRYPT:
				if (seedType != currentSeedType)
					generator.seed(UInt64FromThermalNoise());
				break;
			default:
				std::out_of_range("Unknown seed seedType");
				break;
			}

			if _CONSTEXPR_IF(std::is_floating_point_v<Primitive_t>)
			{
				std::uniform_real_distribution<Primitive_t> distribution(lowerBound, upperBound);
				return distribution(generator);
			}
			else
			{
				std::uniform_int_distribution<Primitive_t> distribution(lowerBound, upperBound);
				return distribution(generator);
			}
		}
	}



#ifdef KLIB_SHORT_NAMESPACE
	using namespace kRng;
#endif
}