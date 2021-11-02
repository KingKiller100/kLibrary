#pragma once

#include "TransformMatrix.hpp"

#include "../../HelperMacros.hpp"
#include "../Vectors/PredefinedVectors.hpp"
#include "../Quaternions/Quaternions.hpp"

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
		res[2][2] = (zDir == ZAxisDirection::LEFT_HAND ? constants::Two<T> : constants::MinusOne<T> *constants::Two<T>)
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
		const T cosA = Cosine(radians);
		const T sinA = Sine(radians);

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
		static constexpr auto zero = constants::Zero<T>;
		static constexpr auto one = constants::One<T>;
		const auto rotate = Rotate(GetTransformIdentity<T>(), radians, { zero, zero, one });
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
		const auto scale = Scale<T>(GetTransformIdentity<T>(), { v[0], v[1], constants::One<T> });
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
		const auto translateMat = Translate<T>(position);
		const auto rotMat = Rotate<T>(radians, axes);
		const auto scaleMat = Scale<T>(scale);
		const TransformMatrix<T> transform = translateMat * rotMat * scaleMat;
		return transform;
	}

	// Translation * Rotation * Scale in 2D
	template<typename T>
	USE_RESULT constexpr TransformMatrix<T> TRS2D(const Vector3<T>& position, const T radians, const Vector2<T>& scale) noexcept
	{
		const auto translateMat = Translate<T>(position);
		const auto rotMat = Rotate2D<T>(radians);
		const auto scaleMat = Scale2D<T>(scale);
		const TransformMatrix<T> transform = translateMat * rotMat * scaleMat;
		return transform;
	}

	template<typename T>
	constexpr bool DecomposeTransform(TransformMatrix<T> const& ModelMatrix, Vector3<T>& Scale, Quaternion<T>& Orientation, Vector3<T>& Translation, Vector3<T>& Skew, Vector4<T>& Perspective)
	{
		TransformMatrix<T> LocalMatrix(ModelMatrix);

		// Normalize the matrix.
		if (ApproximatelyZero(LocalMatrix[3][3]))
			return false;

		for (Length_t i = 0; i < 4; ++i)
			for (Length_t j = 0; j < 4; ++j)
				LocalMatrix[i][j] /= LocalMatrix[3][3];

		// perspectiveMatrix is used to solve for perspective, but it also provides
		// an easy way to test for singularity of the upper 3x3 component.
		Matrix4x4<T> PerspectiveMatrix(LocalMatrix);

		for (Length_t i = 0; i < 3; i++)
			PerspectiveMatrix[i][3] = static_cast<T>(0);
		PerspectiveMatrix[3][3] = static_cast<T>(1);

		/// TODO: Fixme!
		if (ApproximatelyZero(PerspectiveMatrix.GetDeterminant()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			ApproximatelyZero(LocalMatrix[0][3]) ||
			ApproximatelyZero(LocalMatrix[1][3]) ||
			ApproximatelyZero(LocalMatrix[2][3]))
		{
			// rightHandSide is the right hand side of the equation.
			Vector4<T> leftHandSide = LocalMatrix[3];

			// Solve the equation by inverting PerspectiveMatrix and multiplying
			// rightHandSide by the inverse.  (This is the easiest way, not
			// necessarily the best.)
			Matrix4x4<T> InversePerspectiveMatrix = PerspectiveMatrix.Inverse();//   inverse(PerspectiveMatrix, inversePerspectiveMatrix);
			Matrix4x4<T> TransposedInversePerspectiveMatrix = InversePerspectiveMatrix.Transpose();//   transposeMatrix4(inversePerspectiveMatrix, transposedInversePerspectiveMatrix);

			Perspective = leftHandSide * TransposedInversePerspectiveMatrix;
			//  v4MulPointByMatrix(rightHandSide, transposedInversePerspectiveMatrix, perspectivePoint);

			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}
		else
		{
			// No perspective.
			Perspective = Vector4<T>(0, 0, 0, 1);
		}

		// Next take care of translation (easy).
		Translation = Vector3<T>(LocalMatrix[3]);
		LocalMatrix[3] = Vector4<T>(0, 0, 0, LocalMatrix[3].w);

		Vector3<T> Row[3], Pdum3;

		// Now get scale and shear.
		for (Length_t i = 0; i < 3; ++i)
			for (Length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		Scale.x = Row[0].Magnitude();// v3Length(Row[0]);

		Row[0] /= Row[0].Magnitude();

		// Compute XY shear factor and make 2nd row orthogonal to 1st.
		Skew.z = Row[0].DotProduct(Row[1]);
		Row[1] = Row[1] * (Row[0] * -Skew.z);

		// Now, compute Y scale and normalize 2nd row.
		Scale.y = Row[1].Magnitude();
		Row[1] /= Row[1].Magnitude();
		Skew.z /= Scale.y;

		// Compute XZ and YZ shears, orthogonalize 3rd row.
		Skew.y = Row[0].DotProduct(Row[2]);
		Row[2] = Row[2] * Row[0] * -Skew.y;
		Skew.x = Row[1].DotProduct(Row[2]);
		Row[2] = Row[2] * Row[1] * -Skew.x;

		// Next, get Z scale and normalize 3rd row.
		Scale.z = (Row[2].Magnitude());
		Row[2] /= Row[2].Magnitude();
		Skew.y /= Scale.z;
		Skew.x /= Scale.z;

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
		Pdum3 = Row[1].CrossProduct(Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (Row[0].DotProduct(Pdum3) < 0)
		{
			for (Length_t i = 0; i < 3; i++)
			{
				Scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}

		// Now, get the rotations out, as described in the gem.

		// FIXME - Add the ability to return either quaternions (which are
		// easier to recompose with) or Euler angles (rx, ry, rz), which
		// are easier for authors to deal with. The latter will only be useful
		// when we fix https://bugs.webkit.org/show_bug.cgi?id=23799, so I
		// will leave the Euler angle code here for now.

		// ret.rotateY = asin(-Row[0][2]);
		// if (cos(ret.rotateY) != 0) {
		//     ret.rotateX = atan2(Row[1][2], Row[2][2]);
		//     ret.rotateZ = atan2(Row[0][1], Row[0][0]);
		// } else {
		//     ret.rotateX = atan2(-Row[2][0], Row[1][1]);
		//     ret.rotateZ = 0;
		// }

		int i, j, k = 0;
		T root, trace = Row[0].x + Row[1].y + Row[2].z;
		if (trace > static_cast<T>(0))
		{
			root = sqrt(trace + static_cast<T>(1.0));
			Orientation.w = static_cast<T>(0.5) * root;
			root = static_cast<T>(0.5) / root;
			Orientation.v.x = root * (Row[1].z - Row[2].y);
			Orientation.v.y = root * (Row[2].x - Row[0].z);
			Orientation.v.z = root * (Row[0].y - Row[1].x);
		} // End if > 0
		else
		{
			static int Next[3] = { 1, 2, 0 };
			i = 0;
			if (Row[1].y > Row[0].x) i = 1;
			if (Row[2].z > Row[i][i]) i = 2;
			j = Next[i];
			k = Next[j];
			
			int off = 1;

			root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

			Orientation[i + off] = static_cast<T>(0.5) * root;
			root = static_cast<T>(0.5) / root;
			Orientation[j + off] = root * (Row[i][j] + Row[j][i]);
			Orientation[k + off] = root * (Row[i][k] + Row[k][i]);
			Orientation.w = root * (Row[j][k] - Row[k][j]);
		} // End if <= 0

		return true;
	}

}