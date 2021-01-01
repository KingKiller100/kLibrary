#pragma once

#include "../../HelperMacros.hpp"

#include "../kAlgorithms.hpp"

#include "Vector.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

#include <cmath>

namespace kmaths
{
	template<typename T, unsigned short N>
	USE_RESULT Vector<T, N> To_Vector(const T* ptr) noexcept(std::is_copy_assignable_v<T>&& std::is_copy_constructible_v<T>)
	{
		Vector<T, N> v;
		for (auto i = 0; i < N; ++i)
			v[i] = ptr[i];
		return v;
	}

	template<typename T, unsigned short N>
	USE_RESULT decltype(auto) To_Array(const Vector<T, N>& vec) noexcept(std::is_copy_assignable_v<T>&& std::is_copy_constructible_v<T>)
	{
		T arr[N]{ };
		for (size_t i = 0; i < N; ++i)
			arr[i] = vec[i];
		return arr;
	}

	template<typename T>
	USE_RESULT constexpr Vector2<T> Rotate(const Vector2<T>& position, const T angle) noexcept
	{
		if _CONSTEXPR_IF(angle == CAST(T, 0))
			return Vector2<T>();

		const auto rotation = atan2(position.Y(), position.X()) + angle;
		const auto mag = position.Magnitude();
		const auto lXPos = cos(rotation) * mag;
		const auto lYPos = sin(rotation) * mag;
		return Vector2<T>(lXPos, lYPos);
	}

	template<typename T>
	USE_RESULT constexpr Vector2<T> RotateAbout(const Vector2<T>& inCenter, const Vector2<T>& inPosition, const T inAngle) noexcept
	{
		Vector2<T> lRes = inPosition;

		lRes -= inCenter;
		lRes = Rotate(lRes, inAngle);
		lRes += inCenter;

		return lRes;
	}

	template<typename T, unsigned short N>
	USE_RESULT constexpr T AngleBetweenVectors(const Vector<T, N>& v, const Vector<T, N>& u, const bool inDegrees = false) noexcept
	{
		const T angle = u.DotProduct(v) / (v.Magnitude() * u.Magnitude());
		return inDegrees ? ToDegrees(acos(angle)) : acos(angle);
	}
}
