#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T>
	using Matrix3x3 = Matrix<T, 3, 3>;

	using Matrix3x3s = Matrix3x3 <   int    >; // signed integer
	using Matrix3x3f = Matrix3x3 <  float   >; // floating point
	using Matrix3x3d = Matrix3x3 <  double  >; // double floating point
}
