#include "pch.hpp"
#include "Matrix_Test.hpp"

#include "../../Source Files/Maths/Vectors/Vector.hpp"
#include "../../Source Files/Maths/Matrices/MatrixMathsHelper.hpp"
#include "../../Source Files/Maths/Matrices/PredefinedMatrices.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{

	MatricesTester::MatricesTester()
		: Tester("Matrix MxN Test")
	{	}

	MatricesTester::~MatricesTester()
		= default;

	void MatricesTester::Test()
	{
		VERIFY_MULTI_INIT();

		VERIFY_MULTI(DynamicMatrixTest());

		VERIFY_MULTI(AddTest());
		VERIFY_MULTI(DivideTest());
		VERIFY_MULTI(SubtractTest());
		VERIFY_MULTI(MultiplyTest());
		VERIFY_MULTI(InitializerListTest());

		VERIFY_MULTI(ConstexprTest());

		VERIFY_MULTI_END();
	}

	using namespace kmaths;
	bool MatricesTester::DynamicMatrixTest()
	{
		constexpr auto m0 = Matrix<float, 2, 3>(1);
		constexpr auto m1 = Matrix<float, 3, 2>(1);
		//m1.Identity(); Cannot compile since function is only usable for square matrices
		//m1.Inverse();	 Cannot compile since function is only usable for square matrices

		for (auto i = 0u; i < m0.GetRows(); ++i) {
			for (auto j = 0u; j < m0.GetColumns(); ++j)
			{
				VERIFY(m0[i][j] == 1);
				VERIFY(m1[j][i] == 1);
			}
		}

		auto m2 = Matrix<float, 3, 2>(2);
		const auto m3 = m1 - m2;
		const auto m4 = m1 / 10;
		//const auto m5 = m1 / m2; // Cannot compile due to division not being possible between matrices
		const auto m6 = m0 * m2;
		const auto m7 = m2 += m1;

		auto m8 = Matrix<float, 3, 3>(1);
		auto m9 = Matrix<float, 3, 10>(2);

		const auto m10 = m8 * m9;
		//const auto m11 = m8 / m9;
		const auto m12 = Matrix<int, 5, 5>{
			{ 1,  2, 1, 0, 2 },
			{ 4, 11, 8, 0, 1 },
			{ 1,  6, 1, 0, 3 },
			{ 0,  0, 0, 6, 5 },
			{ 3,  5, 7, 6, 4 }
		};

		const auto determinantM12 = m12.GetDeterminant();
		VERIFY(determinantM12 == -96);
		const auto transposedM9 = m12.Transpose();
		for (auto row = 0u; row < m12.GetRows(); ++row)
			for (auto col = 0u; col < m12.GetColumns(); ++col)
				VERIFY(transposedM9[col][row] == m12[row][col]);

		{
			const auto m13 = IdentityMatrix<double, 4, 4>();
			for (auto r = 0u; r < m13.GetRows(); ++r)
				for (auto c = 0u; c < m13.GetColumns(); ++c)
					VERIFY(m13[r][c] == (r == c ? 1 : 0));

			VERIFY(m13.IsIdentity());
		}

		const auto minorMatrix = m12.CreateMinorMatrix(0, 0);
		auto m14 = Matrix<float, 3, 3>();

		auto count = 0;
		for (auto r = 0u; r < m14.GetRows(); ++r)
			for (auto c = 0u; c < m14.GetColumns(); ++c)
				m14[r][c] = (c == 0 && r == 0) ? 10.0f : ++count;
		VERIFY(!m14.IsIdentity());

		constexpr auto size = m14.GetLength();

		const auto inverse3x3 = m14.Inverse();

		const auto identity1 = inverse3x3 * m14;
		const auto identity2 = m14 * inverse3x3;
		for (auto r = 0u; r < m14.GetRows(); ++r) {
			for (auto c = 0u; c < inverse3x3.GetColumns(); ++c)
			{
				VERIFY(identity1[r][c] == (r == c ? 1.0 : 0.0));
				VERIFY(identity2[r][c] == (r == c ? 1.0 : 0.0));
			}
		}

		const Matrix<float, 3, 1> colVec{ {10}, {10}, {10} };
		const auto newVec = m0 * colVec;
		VERIFY(newVec[0][0] == 30.f);
		VERIFY(newVec[1][0] == 30.f);

		const auto newVec2 = inverse3x3 * colVec;
		VERIFY(newVec2[0][0] == 0.f);
		VERIFY(newVec2[1][0] == -10.f);
		VERIFY(newVec2[2][0] == 10.f);

		return success;
	}

	bool MatricesTester::AddTest()
	{
		constexpr auto m1 = Matrix<float, 3, 2>(1.f);
		constexpr auto m2 = Matrix<float, 3, 2>(2.f);
		const auto result = m1 + m2;

		for (auto row = 0u; row < result.GetRows(); ++row) {
			for (auto col = 0u; col < result.GetColumns(); ++col)
			{
				VERIFY(result[row][col] == 3.f);
			}
		}


		return success;
	}

	bool MatricesTester::SubtractTest()
	{
		constexpr auto m1 = Matrix<float, 3, 2>(1.f);
		constexpr auto m2 = Matrix<float, 3, 2>(2.f);
		const auto result = m1 - m2;

		for (auto row = 0u; row < result.GetRows(); ++row) {
			for (auto col = 0u; col < result.GetColumns(); ++col)
			{
				VERIFY(result[row][col] == -1.f);
			}
		}

		return success;
	}

	bool MatricesTester::DivideTest()
	{
		{
			constexpr auto m1 = Matrix<float, 3, 2>(10.f);
			const auto result = m1 / 2.f;

			for (auto row = 0u; row < result.GetRows(); ++row)
				for (auto col = 0u; col < result.GetColumns(); ++col)
					VERIFY(result[row][col] == 5.f);
		}

		{
			const auto m1 = Matrix<float, 3, 3>{
				{3, 6, 1},
				{2, 5, 8},
				{9, 4, 7},
			};
			const auto result = Vector3f(1) / m1;
			const auto x = Round(result.X(), 3);
			const auto y = Round(result.Y(), 3);
			const auto z = Round(result.Z(), 3);


			VERIFY(x == 0.075f);
			VERIFY(y == 0.05f);
			VERIFY(z == 0.075f);
		}


		return success;
	}

	bool MatricesTester::MultiplyTest()
	{
		{
			constexpr auto m1 = Matrix<float, 3, 2>(10.f);
			const auto result = m1 * 2.f;

			for (auto row = 0u; row < result.GetRows(); ++row)
				for (auto col = 0u; col < result.GetColumns(); ++col)
					VERIFY(result[row][col] == 20.f);
		}

		{
			constexpr auto m1 = Matrix<float, 2, 3>(10.f);
			const auto result = Matrix<float, 3, 3>(5) * m1;

			for (auto row = 0u; row < result.GetRows(); ++row)
				for (auto col = 0u; col < result.GetColumns(); ++col)
					VERIFY(result[row][col] == 100.f);
		}

		{
			constexpr auto m1 = Matrix<float, 3, 3>(10.f);
			const auto result = Vector3f(2.f) * m1;

			VERIFY(result.X() == 60.f);
			VERIFY(result.Y() == 60.f);
			VERIFY(result.Z() == 60.f);
		}

		return success;
	}

	bool MatricesTester::InitializerListTest()
	{
		//{
		//	const auto mat = Matrix4x4s{
		//		{1, 1},
		//		{1, 1},
		//		{1, 1},
		//		{1, 1},
		//	};

		//	for (auto row = 0; row < mat.GetRows(); row++) {
		//		for (auto col = 0; col < mat.GetColumns(); col++)
		//		{
		//			const auto expected = col >= 2 ? 0 : 1;
		//			VERIFY(mat[row][col] == expected);
		//		}
		//	}
		//}

		//{
		//	const auto mat = Matrix4x4s{
		//		{1, 1, 1, 1},
		//		{1, 1, 1, 1},
		//	};

		//	for (auto row = 0; row < mat.GetRows(); row++) {
		//		for (auto col = 0; col < mat.GetColumns(); col++)
		//		{
		//			const auto expected = ((row * mat.GetColumns()) + col) > 7 ? 0 : 1;
		//			VERIFY(mat[row][col] == expected);
		//		}
		//	}
		//}


		{
			constexpr int arr[16] = {
					1, 1, 1, 1,
					1, 1, 1, 1,
					0, 0, 0, 0,
			};
			
			const auto mat = Matrix4x4s(arr);

			for (auto row = 0; row < mat.GetRows(); row++) {
				for (auto col = 0; col < mat.GetColumns(); col++)
				{
					const auto expected = ((row * mat.GetColumns()) + col) > 7 ? 0 : 1;
					VERIFY(mat[row][col] == expected);
				}
			}
		}

		return success;
	}

	bool MatricesTester::ConstexprTest()
	{
		// Currently constexpr supported functions for matrices

		constexpr auto v = Vector<double, 3>(10.0, 10.0, 10.0);
		constexpr auto m15 = Matrix<double, 3, 3>{
			{1, 2, 3},
			{3, 4, 6},
			{7, 8, 9},
		};

		const auto m2v = v * m15;
		VERIFY(m2v.X() == 110.f);
		VERIFY(m2v.Y() == 140.f);
		VERIFY(m2v.Z() == 180.f);

		const auto m2v2 = v / m15;
		constexpr auto accuracy = 8;
		VERIFY(Round<double>(m2v2.X(), accuracy) == -Round(5.0 / 3, accuracy));
		VERIFY(m2v2.Y() == 0);
		VERIFY(Round<double>(m2v2.Z(), accuracy) == Round(5.0 / 3, accuracy));

		UNUSED constexpr auto mirror = m15.Mirror();
		UNUSED constexpr auto transpose = m15.Transpose();
		UNUSED constexpr auto miniMatrix = m15.CreateMinorMatrix(0, 0);
		UNUSED constexpr auto determinant = m15.GetDeterminant();
		UNUSED constexpr auto inverse = m15.Inverse();
		UNUSED constexpr auto isIdentity = m15.IsIdentity();
		UNUSED constexpr auto numberOfElems = m15.GetLength();
		UNUSED constexpr auto numberOfRows = m15.GetRows();
		UNUSED constexpr auto numberOfColumns = m15.GetColumns();
		UNUSED constexpr auto isSquare = m15.IsSquare();
		UNUSED constexpr auto isZero = m15.IsZero();
		UNUSED constexpr auto powerOfTwo = m15.PowerOf(2);
		// constexpr auto pointer = m15.GetPointerToData(); // Not supported til c++20

		UNUSED constexpr auto dummyMat = Matrix<double, 3, 3>(1.0 / 128);
		UNUSED constexpr auto add = m15 + dummyMat;
		UNUSED constexpr auto multiple = m15 * dummyMat;
		UNUSED constexpr auto scalarDiv = m15 / 2;
		UNUSED constexpr auto subtract = m15 - dummyMat;
		UNUSED constexpr auto scalarMul = m15 * 5;

		return success;
	}
}
#endif