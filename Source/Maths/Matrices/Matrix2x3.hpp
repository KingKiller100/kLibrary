#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix2x3 = Matrix<T, 2, 3>;

	using Matrix2x3s = Matrix2x3 <   int    >; // 2 rows - 3 columns - signed integer
	using Matrix2x3f = Matrix2x3 <  float   >; // 2 rows - 3 columns - floating point
	using Matrix2x3d = Matrix2x3 <  double  >; // 2 rows - 3 columns - double floating point
}
