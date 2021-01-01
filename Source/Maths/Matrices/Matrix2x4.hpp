#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix2x4 = Matrix<T, 2, 4>;

	using Matrix2x4s = Matrix2x4 <   int    >; // 2 rows - 4 columns - signed integer
	using Matrix2x4f = Matrix2x4 <  float   >; // 2 rows - 4 columns - floating point
	using Matrix2x4d = Matrix2x4 <  double  >; // 2 rows - 4 columns - double floating point
}
