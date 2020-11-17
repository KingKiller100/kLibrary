#pragma once

#include "GenericSeeding.hpp"
#include "../../HelperMacros.hpp"
#include <random>

namespace kmaths
{
	namespace kRng
	{
		enum class RngBitSize
		{
			Bit32 = 32,
			Bit64 = 64,
			Unknown,
		};
		
		template<RngBitSize BitSize>
		class Rng { constexpr Rng() = delete; };

		template<>
		class Rng<RngBitSize::Bit32>
		{
		public:
			using Seed_t = std::uint32_t;
			static constexpr auto BitSize = RngBitSize::Bit32;

			explicit Rng(GenericSeedingSource seedingSourceType)
			{
				const auto seedVal = U32Seed(seedingSourceType);
				generator.seed(seedVal);
			}

			explicit Rng(const Seed_t seed)
			{
				generator.seed(seed);
			}

			~Rng() noexcept = default;

			template<typename Primitive_t, typename = std::enable_if_t<
				std::is_arithmetic_v<Primitive_t>
				>>
				Primitive_t operator()(Primitive_t lowerBound, Primitive_t upperBound) noexcept
			{
				return Generate(lowerBound, upperBound);
			}

			template<typename Primitive_t, typename = std::enable_if_t<
				std::is_arithmetic_v<Primitive_t>
			>>
			Primitive_t Generate(Primitive_t lowerBound, Primitive_t upperBound) noexcept
			{
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

		private:
			std::mt19937 generator;
		};
		
		template<>
		class Rng<RngBitSize::Bit64>
		{
		public:
			using Seed_t = std::uint64_t;
			static constexpr auto BitSize = RngBitSize::Bit32;

			explicit Rng(GenericSeedingSource seedingSourceType)
			{
				const auto seedVal = U64Seed(seedingSourceType);
				generator.seed(seedVal);
			}

			explicit Rng(const Seed_t seed)
			{
				generator.seed(seed);
			}

			~Rng() noexcept = default;

			template<typename Primitive_t, typename = std::enable_if_t<
				std::is_arithmetic_v<Primitive_t>
				>>
				Primitive_t operator()(Primitive_t lowerBound, Primitive_t upperBound) noexcept
			{
				return Generate(lowerBound, upperBound);
			}

			template<typename Primitive_t, typename = std::enable_if_t<
				std::is_arithmetic_v<Primitive_t>
			>>
			USE_RESULT Primitive_t Generate(Primitive_t lowerBound, Primitive_t upperBound) noexcept
			{
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
			
		private:
			std::mt19937_64 generator;
		};
		
		using Rng32 = Rng<RngBitSize::Bit32>;
		using Rng64 = Rng<RngBitSize::Bit64>;

	}
	
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kRng;
#endif
}