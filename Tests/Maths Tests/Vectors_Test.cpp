#include "pch.hpp"
#include "Vectors_Test.hpp"

#include "../../Source Files/Maths/Vectors/VectorMathsHelper.hpp"
#include "../../Source Files/Maths/Vectors/Vector2.hpp"
#include "../../Source Files/Maths/Vectors/Vector3.hpp"
#include "../../Source Files/Maths/Vectors/Vector4.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	using namespace kmaths;

	VectorsTester::VectorsTester()
		: Tester("Vectors X/2/3/4 Test")
	{ }

	VectorsTester::~VectorsTester()
		= default;

	void VectorsTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(VectorTest());
		VERIFY_MULTI(Vector2Test());
		VERIFY_MULTI(Vector3Test());
		VERIFY_MULTI(Vector4Test());
		VERIFY_MULTI_END();
	}

	bool VectorsTester::VectorTest()
	{
		auto vec2f = Vector<float, 2>(4.f);
		auto vec2d = Vector<double, 2>{ 5, 12 };

		auto res1 = (vec2f + vec2d);
		VERIFY(res1.X() == 9.0 && res1.Y() == 16.0);
		res1 = vec2f - vec2d;
		VERIFY(res1.X() == -1.0 && res1.Y() == -8.0);
		res1 = vec2f * vec2d;
		VERIFY(res1.X() == 20 && res1.Y() == 48);
		res1 = vec2f / vec2d;
		VERIFY(res1.X() == 0.8f && res1.Y() == 1.f / 3.f);

		const auto distance = vec2f.Distance(vec2d);
		VERIFY(distance == 8.06225777f);
		vec2f = vec2f.Normalize();
		VERIFY(vec2f.X() == 0.707106769f && vec2f.Y() == 0.707106769f);

		constexpr auto length1 = Vector<double, 3>(41, 55, 67);
		constexpr auto length2 = Vector<double, 3>(14, 55, 76);

		const auto rads = AngleBetweenVectors(length1, length2);
		const auto degs = AngleBetweenVectors(length1, length2, true);

		const auto toDegs = ToDegrees(rads);
		const auto toRads = ToRadians(degs);

		VERIFY(Round(degs, 16) == Round(toDegs, 16));
		VERIFY(Round(rads, 16) == Round(toRads, 16));

		vec2d = vec2f;
		vec2f += vec2d;
		vec2f -= vec2d;
		vec2f *= vec2d;

		const bool greaterThan = vec2d > 14;
		VERIFY(greaterThan == false);
		const bool lessThan = vec2d < 14;
		VERIFY(lessThan == true);

		// Currently constexpr supported functions
		constexpr auto vec2s = Vector<int, 2>(5, 12);
		constexpr auto vec2s2 = Vector<int, 2>(2);
		constexpr Vector<int, 3> copy = vec2s2;
		constexpr auto dp = vec2s.DotProduct(vec2s2);
		VERIFY(dp == 34);

		constexpr auto vec3s1 = Vector<int, 3>(1, 2, 3);
		constexpr auto vec3s2 = Vector<int, 3>(4, 5, 6);
		constexpr auto vec3l = Vector<long, 3>(7, 8, 9);

		constexpr auto magSq = vec3s1.MagnitudeSQ();
		VERIFY(magSq == 14);

		constexpr auto x = vec3s1.X();
		constexpr auto y = vec3s1.Y();
		constexpr auto z = vec3s1.Z();

		//const auto wFail = vec3s1.W(); // unable to compile this line as vector3 do not have W axis

		constexpr auto length2D = Vector2d(7, 16);
		constexpr auto perp = length2D.Perpendicular();
		VERIFY(static_cast<int>(perp.X()) == -16 && static_cast<int>(perp.Y()) == 7);
		//const auto perpFail = vec3s1.Perpendicular(); // unable to compile due to having more than only 2 axes
		constexpr auto crossP = vec3s2.CrossProduct(vec3l);
		VERIFY(crossP.X() == -3 && crossP.Y() == 6 && crossP.Z() == -3);
		//const auto crossPFail = vec2s.CrossProduct(vec2s); // unable to compile due to not having only 3 axes

		auto vec4u = Vector<unsigned, 4>();
		constexpr char chars[] = { 'H', 'E', 'L', 'P', '\0' };
		constexpr auto vec5c = Vector<char, 5>(chars);

		constexpr auto w = vec5c.W();
		VERIFY_COMPILE_TIME(w == 'P');

		vec4u += vec3l;
		VERIFY(vec4u.x == 7);
		VERIFY(vec4u.y == 8);
		VERIFY(vec4u.z == 9);
		VERIFY(vec4u.w == 0);

		vec4u -= Vector2d(2, 1);
		VERIFY(vec4u.x == 5);
		VERIFY(vec4u.y == 7);
		VERIFY(vec4u.z == 9);
		VERIFY(vec4u.w == 0);

		constexpr auto vecULL = Vector<unsigned long long, 30>(100);
		for (auto i = 0; i < vecULL.GetLength(); ++i)
			VERIFY(vecULL[i] == 100);

		constexpr bool bools[] = { true, false, false, true, false, false, true };
		constexpr auto vecBool = Vector<bool, 7>(bools);
		constexpr auto b6 = vecBool[6];
		VERIFY_COMPILE_TIME(b6 == true);

		constexpr auto lhs = Vector2d(10, 5);
		constexpr auto rhs = Vector2d(9, 15);

		// Operators
		constexpr auto add = lhs + rhs;
		VERIFY_COMPILE_TIME(add.At(0) == 19.0);
		VERIFY_COMPILE_TIME(add.At(1) == 20.0);
		constexpr auto subtract = lhs - rhs;
		VERIFY_COMPILE_TIME(subtract.At(0) == 1.0);
		VERIFY_COMPILE_TIME(subtract.At(1) == -10.0);
		constexpr auto multiply = lhs * rhs;
		VERIFY_COMPILE_TIME(multiply.At(0) == 90.0);
		VERIFY_COMPILE_TIME(multiply.At(1) == 75.0);
		constexpr auto divide = lhs / rhs;
		VERIFY_COMPILE_TIME(divide.At(0) == 1.1111111111111112);
		VERIFY_COMPILE_TIME(divide.At(1) == 0.33333333333333331);
		constexpr auto noEqual = lhs != rhs;
		VERIFY_COMPILE_TIME(noEqual)

			// Functions
			constexpr auto magnitudeSq = lhs.MagnitudeSQ();
		VERIFY_COMPILE_TIME(magnitudeSq == 125);
		constexpr auto magnitude = lhs.Magnitude();
		VERIFY_COMPILE_TIME(magnitude == 11.180339887498949);
		constexpr auto normalize = lhs.Normalize();
		constexpr auto dist = lhs.Distance(rhs);
		constexpr auto dotProduct = lhs.DotProduct(rhs);
		VERIFY_COMPILE_TIME(dotProduct == 165);
		constexpr auto dimensions = lhs.GetLength();
		VERIFY_COMPILE_TIME(dimensions == 2);
		constexpr auto reverseVector = lhs.Reverse();
		VERIFY_COMPILE_TIME(reverseVector.X() == -10 && reverseVector.Y() == -5);
		constexpr auto inverse = lhs.Inverse();
		VERIFY_COMPILE_TIME(inverse.X() == 0.1 && inverse.Y() == 0.2);
		constexpr auto isZero = lhs.IsZero();
		VERIFY_COMPILE_TIME(isZero == false);
		constexpr auto perpendicular = lhs.Perpendicular();
		VERIFY_COMPILE_TIME(perpendicular.X() == -5 && perpendicular.Y() == 10);

		constexpr auto lhs3d = Vector3d(10, 5, 3);
		constexpr auto rhs3d = Vector3d(9, 15, 23);

		constexpr auto crossProduct = lhs3d.CrossProduct(rhs3d);
		VERIFY_COMPILE_TIME(crossProduct.X() == 70);
		VERIFY_COMPILE_TIME(crossProduct.y == -203);
		VERIFY_COMPILE_TIME(crossProduct.Z() == 105);

		constexpr auto compareVal = 15;
		constexpr bool greater = (lhs3d >= compareVal);
		VERIFY_COMPILE_TIME(greater == false);
		constexpr bool less = lhs3d <= compareVal;
		VERIFY_COMPILE_TIME(less == true);

		return success;
	}

	bool VectorsTester::Vector2Test()
	{
		constexpr auto vec2f = Vector2f(4.f);
		constexpr auto vec2d = Vector2d(5, 12);

		auto res1 = (vec2f + vec2d);
		VERIFY(res1.x == 9.0 && res1.y == 16.0);
		res1 = vec2f - vec2d;
		VERIFY(res1.x == -1.0 && res1.y == -8.0);
		res1 = vec2f * vec2d;
		VERIFY(res1.x == 20 && res1.y == 48);
		res1 = vec2f / vec2d;
		VERIFY(res1.x == 0.8f && res1.y == 1.f / 3.f);

		constexpr auto distance = vec2f.Distance(vec2d);
		VERIFY(distance == 8.06225777f);
		constexpr auto normal = vec2f.Normalize();
		VERIFY(normal.x == 0.707106769f && normal.y == 0.707106769f);

		constexpr auto x = vec2f.x;
		VERIFY(x == 4.f);
		constexpr auto y = vec2f.y;
		VERIFY(y == 4.f);
		//const auto z = vec2f.z; // Can't compile since z doesn't exist in 2 dimensions
		//const auto w = vec2f.w; // Can't compile since w doesn't exist in 2 dimensions

		constexpr Vector2s data(1, 2);
		const auto ptr = data.GetPointerToData();
		const auto value = ptr[0];
		VERIFY(value == data.x);

		constexpr auto first = vec2d.At(0);
		VERIFY_COMPILE_TIME(first == 5.0);

		constexpr auto perp = vec2d.Perpendicular();
		VERIFY_COMPILE_TIME(perp.x == -12.0);
		VERIFY_COMPILE_TIME(perp.y == 5.0);


		return success;
	}

	bool VectorsTester::Vector3Test()
	{
		constexpr Vector3d data(5, 6, 7);
		const auto ptr = data.GetPointerToData();
		const auto value = ptr[1];
		VERIFY(value == data.y);

		constexpr Vector3d d2(1, 2, 3);
		constexpr auto cross = d2.CrossProduct(data);
		VERIFY_COMPILE_TIME(cross.x == -4);
		VERIFY_COMPILE_TIME(cross.y == 8);
		VERIFY_COMPILE_TIME(cross.z == -4);

		return success;
	}

	bool VectorsTester::Vector4Test()
	{
		constexpr Vector4d vector4(2, 2, 2, 2);
		const auto ptr = vector4.GetPointerToData();
		const auto value = ptr[3];
		VERIFY(value == vector4.w);

		VERIFY(vector4 == 4);

		return success;
	}
}
#endif
