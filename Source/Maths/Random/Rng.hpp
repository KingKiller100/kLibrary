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
			BitU32 = 32,
			BitU64 = 64,
			BitS32 = ~32,
			BitS64 = ~64,
			Unknown,
		};
		
		template<RngBitSize BitSize>
		class Rng { constexpr Rng() = delete; };

		template<>
		class Rng<RngBitSize::BitU32>
		{
		public:
			using Seed_t = std::uint32_t;
			static constexpr auto BitSize = RngBitSize::BitU32;

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
		class Rng<RngBitSize::BitS32>
		{
		public:
			using Seed_t = std::int32_t;
			static constexpr auto BitSize = 32;

			explicit Rng(GenericSeedingSource seedingSourceType)
			{
				const auto seedVal = S32Seed(seedingSourceType);
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
		class Rng<RngBitSize::BitU64>
		{
		public:
			using Seed_t = std::uint64_t;
			static constexpr auto BitSize = RngBitSize::BitU64;

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
		
		template<>
		class Rng<RngBitSize::BitS64>
		{
		public:
			using Seed_t = std::int64_t;
			static constexpr auto BitSize = 64;

			explicit Rng(GenericSeedingSource seedingSourceType)
			{
				const auto seedVal = S64Seed(seedingSourceType);
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
		
		using RngU32 = Rng<RngBitSize::BitU32>;
		using RngU64 = Rng<RngBitSize::BitU64>;
		
		using RngS32 = Rng<RngBitSize::BitS32>;
		using RngS64 = Rng<RngBitSize::BitS64>;

	}
	
#ifdef KLIB_SHORT_NAMESPACE
	using namespace kRng;
#endif
}