#pragma once
#include "../../HelperMacros.hpp"

#include <cstdint>

namespace kmaths
{
	namespace kRng
	{
		enum class GenericSeedingSource
		{
			TIME,
			BCRYPT,
			UNKNOWN,
		};

		USE_RESULT std::uint64_t U64Seed(GenericSeedingSource sourceType);
		USE_RESULT std::uint32_t U32Seed(GenericSeedingSource sourceType);
	}
	
#ifdef KLIB_SHORT_NAMESPACE
		using namespace kRng;
#endif
}
