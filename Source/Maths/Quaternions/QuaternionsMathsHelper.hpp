#pragma once
#include "Quaternions.hpp"

#include "../../HelperMacros.hpp"


namespace kmaths
{
	template<typename T>
	USE_RESULT constexpr Quaternion<T> IdentityQuaternion(Theta thetaType) noexcept
	{
		constexpr auto one = constants::numbers::One<T>;
		constexpr auto zero = constants::numbers::Zero<T>;
		
		return Quaternion<T>(one, zero, zero, zero, thetaType);
	}
}
