#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix4x3 = Matrix<T, 4, 3>;

	using Matrix4x3s = Matrix4x3 <   int    >; // 4 rows - 3 columns - signed integer
	using Matrix4x3f = Matrix4x3 <  float   >; // 4 rows - 3 columns - floating point
	using Matrix4x3d = Matrix4x3 <  double  >; // 4 rows - 3 columns - double floating point
}
