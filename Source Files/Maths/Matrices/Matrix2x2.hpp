#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix2x2 = Matrix<T, 2, 2>;

	using Matrix2x2s = Matrix2x2 <   int    >; // signed integer
	using Matrix2x2f = Matrix2x2 <  float   >; // floating point
	using Matrix2x2d = Matrix2x2 <  double  >; // double floating point
}
