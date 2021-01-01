#pragma once 

#include "Matrix.hpp"

namespace kmaths
{
	template<class T> using Matrix4x4 = Matrix<T, 4, 4>;

	using Matrix4x4s = Matrix4x4 <   int    >; // signed integer
	using Matrix4x4f = Matrix4x4 <  float   >; // floating point
	using Matrix4x4d = Matrix4x4 <  double  >; // double floating point
}
