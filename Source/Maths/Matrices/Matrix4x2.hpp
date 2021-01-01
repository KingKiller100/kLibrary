#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix4x2 = Matrix<T, 4, 2>;

	using Matrix4x2s = Matrix4x2 <   int    >; // 4 rows - 2 columns - signed integer
	using Matrix4x2f = Matrix4x2 <  float   >; // 4 rows - 2 columns - floating point
	using Matrix4x2d = Matrix4x2 <  double  >; // 4 rows - 2 columns - double floating point
}
