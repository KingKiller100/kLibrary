#pragma once

#include "TransformMatrix.hpp"

#include "../../HelperMacros.hpp"
#include "../Vectors/PredefinedVectors.hpp"

#include <cstdint>

namespace kmaths
{
	template<typename T, unsigned short Rows, unsigned short Columns>
	USE_RESULT Matrix<T, Rows, Columns> ToMatrix(const T(&arr)[(Rows * Columns)]) noexcept(std::is_copy_assignable_v<T>&& std::is_copy_constructible_v<T>)
	{
		Matrix<T, Rows, Columns> m(arr);
		return m;
	}

	template<typename T, unsigned short Rows, unsigned short Columns>
	USE_RESULT Matrix<T, Rows, Columns> ToMatrix(const T(&arr)[Rows][Columns]) noexcept(std::is_copy_assignable_v<T>&& std::is_copy_constructible_v<T>)
	{
		Matrix<T, Rows, Columns> m(arr);
		return m;
	}

	template<typename T, unsigned short Rows, unsigned short Columns>
	USE_RESULT decltype(auto) ToArray(const Matrix<T, Rows, Columns>& mat) noexcept(std::is_copy_assignable_v<T>&& std::is_copy_constructible_v<T>)
	{
		T arr[Rows][Columns]{};

		for (Length_t row = 0; row < Rows; ++row)
			for (Length_t col = 0; col < Columns; ++col)
				arr[row][col] = mat[row][col];
		return arr;
	}


	enum class ZAxisDirection : uint8_t
	{
		LEFT_HAND,
		RIGHT_HAND
	};

	template<typename T>
	USE_RESULT static constexpr const TransformMatrix<T>& GetTransformIdentity() noexcept
	{
		static constexpr auto identity = IdentityMatrix<T, 4, 4>();
		return identity;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Ortho(const T left, const T right, const T bottom, const T top) noexcept
	{
		auto res = GetTransformIdentity<T>();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = CAST(T, -1);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(top + bottom) / (top - bottom);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Ortho_ZO(const ZAxisDirection zDir, const T left, const T right
		, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = GetTransformIdentity<T>();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (zDir == ZAxisDirection::LEFT_HAND ? constants::One<T> : constants::MinusOne<T>)
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(top + bottom) / (top - bottom);
		res[3][2] = -zNear / (zFar - zNear);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Ortho_NO(const ZAxisDirection zDir, const T left, const T right
		, const T bottom, const T top, const T zNear, const T zFar) noexcept
	{
		auto res = GetTransformIdentity<T>();
		res[0][0] = CAST(T, 2) / (right - left);
		res[1][1] = CAST(T, 2) / (top - bottom);
		res[2][2] = (zDir == ZAxisDirection::LEFT_HAND ? constants::Two<T> : constants::MinusOne<T> * constants::Two<T>)
			/ (zFar - zNear);
		res[3][0] = -(right + left) / (right - left);
		res[3][1] = -(top + bottom) / (top - bottom);
		res[3][2] = -(zFar + zNear) / (zFar - zNear);
		return res;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Perspective_ZO(const ZAxisDirection zDir, T fov, T aspectRatio, T zNear, T zFar) noexcept
	{
		constexpr auto one = constants::One<T>;
		constexpr auto two = constants::Two<T>;

		const auto halfFov = fov / two;
		const T tanHalfFov = Tan(halfFov);

		const auto denom23 = (zDir == ZAxisDirection::LEFT_HAND)
			? (zFar - zNear)
			: (zNear - zFar);

		TransformMatrix<T> mat;
		mat[0][0] = constants::OneOver<T>(aspectRatio * tanHalfFov);
		mat[1][1] = constants::OneOver<T>(tanHalfFov);
		mat[2][2] = zFar / denom23;
		mat[2][3] = (zDir == ZAxisDirection::LEFT_HAND) ? one : -one;
		mat[3][2] = -(zFar * zNear) / (zFar - zNear);
		return mat;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Perspective_NO(const ZAxisDirection zDir, T fov, T aspectRatio, T zNear, T zFar) noexcept
	{
		constexpr auto one = constants::One<T>;
		constexpr auto two = constants::Two<T>;

		const auto halfFov = fov / two;
		const T tanHalfFov = Tan(halfFov);

		TransformMatrix<T> mat;
		mat[0][0] = constants::OneOver<T>(aspectRatio * tanHalfFov);
		mat[1][1] = constants::OneOver<T>(tanHalfFov);
		mat[2][2] = (zFar + zNear) / (zFar - zNear);
		mat[2][3] = (zDir == ZAxisDirection::LEFT_HAND) ? one : -one;
		mat[3][2] = -(two * zFar * zNear) / (zFar - zNear);
		return mat;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Translate(const TransformMatrix<T>& m, const Vector3<T>& v)
	{
		TransformMatrix<T> translate = m;
		translate[3] = (m[0] * v[0]) + (m[1] * v[1]) + (m[2] * v[2]) + m[3];
		return translate;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Translate(const Vector3<T>& v) noexcept
	{
		const auto translate = Translate<T>(GetTransformIdentity<T>(), v);
		return translate;
	}

	// Rotate transform in radians
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Rotate(const TransformMatrix<T>& m, T radians, const Vector3<T>& axes) noexcept
	{
		const auto a = radians;

		const T cosA = Cosine(a);
		const T sinA = Sine(a);

		Vector3<T> axis = axes.Normalize();
		Vector3<T> temp = axis * (constants::One<T> -cosA);

		TransformMatrix<T> rotate;
		rotate[0][0] = cosA + temp[0] * axis[0];
		rotate[0][1] = temp[0] * axis[1] + sinA * axis[2];
		rotate[0][2] = temp[0] * axis[2] - sinA * axis[1];

		rotate[1][0] = temp[1] * axis[0] - sinA * axis[2];
		rotate[1][1] = cosA + temp[1] * axis[1];
		rotate[1][2] = temp[1] * axis[2] + sinA * axis[0];

		rotate[2][0] = temp[2] * axis[0] + sinA * axis[1];
		rotate[2][1] = temp[2] * axis[1] - sinA * axis[0];
		rotate[2][2] = cosA + temp[2] * axis[2];

		TransformMatrix<T> res;
		res[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
		res[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
		res[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
		res[3] = m[3];

		return res;
	}

	// Rotate transform in radians
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Rotate(T radians, const Vector3<T>& axes) noexcept
	{
		const auto rotate = Rotate(GetTransformIdentity<T>(), radians, axes);
		return rotate;
	}

	// Rotate transform in radians
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Rotate2D(T radians) noexcept
	{
		const auto rotate = Rotate(GetTransformIdentity<T>(), radians, { CAST(T, 0), CAST(T, 0), constants::One<T> });
		return rotate;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale(const TransformMatrix<T>& m, const Vector3<T>& v) noexcept
	{
		return TransformMatrix<T>{
			(m[0] * v[0]),
				(m[1] * v[1]),
				(m[2] * v[2]),
				m[3]
		};
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale(const Vector3<T>& v) noexcept
	{
		const auto scale = Scale<T>(GetTransformIdentity<T>(), v);
		return scale;
	}

	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> Scale2D(const Vector2<T>& v) noexcept
	{
		const auto scale = Scale<T>(GetTransformIdentity<T>(), { v[0], v[1], CAST(T, 1) });
		return scale;
	}

	/**
	 * \brief
	 *		Returns transform matrix made from Translation * Rotation * Scale
	 * \tparam T
	 *		Type
	 * \param position
	 *		3D Vector
	 * \param radians
	 *		amount to rotate
	 * \param axes
	 *		Axes to affect
	 * \param scale
	 *		Size of each component of the position
	 * \return
	 *		Complete transform matrix representing all three transformations taking place on a coordinate
	 */
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> TRS(const Vector3<T>& position, const T radians, const Vector3<T>& axes, const Vector3<T>& scale) noexcept
	{
		const TransformMatrix<T> transform = Translate<T>(position) * Rotate<T>(radians, axes) * Scale<T>(scale);
		return transform;
	}

	// Translation * Rotation * Scale in 2D
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> TRS2D(const Vector3<T>& position, const T radians, const Vector2<T>& scale) noexcept
	{
		const TransformMatrix<T> transform = Translate<T>(position) * Rotate2D<T>(radians) * Scale2D<T>(scale);
		return transform;
	}

	template<typename T>
	constexpr bool DecomposeTransform(TransformMatrix<T> m, Vector3<T>& position, Vector3<T>& rotation, Vector3<T>& scale) noexcept(std::is_arithmetic_v<T>)
	{
		using namespace constants;

		static constexpr auto zero = Zero<T>;
		static constexpr auto one = One<T>;

		if (ApproximatelyZero<T>(m[3][3]))
		{
			return false;
		}

		// Isolate perspective [messy]
		if (
			ApproximatelyZero<T>(m[3][0])
			|| ApproximatelyZero<T>(m[3][1])
			|| ApproximatelyZero<T>(m[3][2])
			)
		{
			// Clear perspective partition
			m[3].Zero();
			m[3][3] = one;
		}

		position = m[3];
		m[3][0] = m[3][1] = m[3][2] = zero;

		Vector3<T> columns[3], pDum3;

		for (Length_t row = 0; row < 3; ++row)
			for (Length_t col = 0; col < 3; ++col)
			{
				columns[row][col] = m[row][col];
			}

		scale.x = columns[0].MagnitudeSQ();
		scale.y = columns[1].MagnitudeSQ();
		scale.z = columns[2].MagnitudeSQ();
		
		columns[0] = columns[0].Normalize();
		columns[1] = columns[1].Normalize();
		columns[2] = columns[2].Normalize();

		rotation.y = std::asin(-columns[2][0]);

		if (cos(rotation.y) != 0) {
			rotation.x = atan2(columns[1][2], columns[2][2]);
			rotation.z = atan2(columns[0][1], columns[0][0]);
		}
		else {
			rotation.x = atan2(-columns[2][0], columns[1][1]);
			rotation.z = zero;
		}

		return true;
	}

}