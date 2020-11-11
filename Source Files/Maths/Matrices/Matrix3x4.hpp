#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix3x4 = Matrix<T, 3, 4>;

	using Matrix3x4s = Matrix3x4 <   int    >; // 3 rows - 4 columns - signed integer
	using Matrix3x4f = Matrix3x4 <  float   >; // 3 rows - 4 columns - floating point
	using Matrix3x4d = Matrix3x4 <  double  >; // 3 rows - 4 columns - double floating point
}
