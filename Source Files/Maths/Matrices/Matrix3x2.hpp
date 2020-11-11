#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix3x2 = Matrix<T, 3, 2>;

	using Matrix3x2s = Matrix3x2 <   int    >; // 3 rows - 2 columns - signed integer
	using Matrix3x2f = Matrix3x2 <  float   >; // 3 rows - 2 columns - floating point
	using Matrix3x2d = Matrix3x2 <  double  >; // 3 rows - 2 columns - double floating point
}
