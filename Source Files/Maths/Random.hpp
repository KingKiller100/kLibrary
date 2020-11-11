#pragma once

#include "../HelperMacros.hpp"

#include <random>
#include <ctime>

namespace kmaths
{
	/**
	 * \brief
	 *		Random number generator
	 * \tparam T
	 *		Return type and param type
	 * \param lowerBound
	 *		Lower bound
	 * \param upperBound
	 *		Upper bound
	 * \return
	 *		Random number between the given bounds
	 */
	template<typename T>
	USE_RESULT static T RNG(T lowerBound, T upperBound)
	{
		static std::mt19937_64 generator (std::time(nullptr));
		
		if _CONSTEXPR_IF(std::is_floating_point_v<T>)
		{
			std::uniform_real_distribution<T> distribution(lowerBound, upperBound);
			return distribution(generator);
		}
		else
		{
			std::uniform_int_distribution<T> distribution(lowerBound, upperBound);
			return distribution(generator);
		}
	}
}
