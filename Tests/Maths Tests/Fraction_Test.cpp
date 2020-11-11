#include "pch.hpp"
#include "Fraction_Test.hpp"

#include "../../Source Files/Maths/Constants.hpp"
#include "../../Source Files/Maths/Fraction.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{

	FractionTester::FractionTester()
		: Tester("Fraction Test")
	{}

	FractionTester::~FractionTester()
		= default;

	using namespace kmaths;

	void FractionTester::Test()
	{
		VERIFY_MULTI_INIT();

		VERIFY_MULTI(AddTest());
		VERIFY_MULTI(DivideTest());
		VERIFY_MULTI(GetRealTest());
		VERIFY_MULTI(SubractTest());
		VERIFY_MULTI(MultiplyTest());
		VERIFY_MULTI(NonSimplifiedTest());
		
		VERIFY_MULTI(RationalTest());
		VERIFY_MULTI(IrrationalTest());

		VERIFY_MULTI_END();
	}

	bool FractionTester::AddTest()
	{
		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = 10;
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 10);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr auto f2 = -2;
			constexpr auto res = f1 + f2;

			VERIFY(res.numerator == 2);
			VERIFY(res.denominator == 1);
			VERIFY(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = { 10, 20, false };
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 1);
			VERIFY_COMPILE_TIME(res.denominator == 2);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 12, 9, false };
			constexpr Fraction f2 = { 10, 20, false };
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 11);
			VERIFY_COMPILE_TIME(res.denominator == 6);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 120, 45, false };
			constexpr Fraction f2 = { 180, 270, false };
			constexpr auto res = f1 + f2;

			VERIFY_COMPILE_TIME(res.numerator == 10);
			VERIFY_COMPILE_TIME(res.denominator == 3);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 120, 45, true };
			constexpr Fraction f2 = { 180, 270, false };
			constexpr auto res = f1 + f2;

			VERIFY(res.numerator == 2);
			VERIFY(res.denominator == 1);
			VERIFY(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		return success;
	}

	bool FractionTester::SubractTest()
	{
		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = { 10, 1, false };
			constexpr auto res = f1 - f2;

			VERIFY_COMPILE_TIME(res.numerator == 10);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = { 10, 5, true };
			constexpr auto res = f1 - f2;

			VERIFY(res.numerator == 2);
			VERIFY(res.denominator == 1);
			VERIFY(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 0, 1, false };
			constexpr Fraction f2 = { 10, 20, false };
			constexpr auto res = f1 - f2;

			VERIFY_COMPILE_TIME(res.numerator == 1);
			VERIFY_COMPILE_TIME(res.denominator == 2);
			VERIFY_COMPILE_TIME(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 12, 9, false };
			constexpr Fraction f2 = { 10, 20, false };
			constexpr auto res = f1 - f2;

			VERIFY_COMPILE_TIME(res.numerator == 5);
			VERIFY_COMPILE_TIME(res.denominator == 6);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 120, 45, false };
			constexpr Fraction f2 = { 180, 270, false };
			constexpr auto res = f1 - f2;

			VERIFY_COMPILE_TIME(res.numerator == 2);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 120, 45, true };
			constexpr Fraction f2 = { 180, 270, false };
			constexpr auto res = f1 - f2;

			VERIFY_COMPILE_TIME(res.numerator == 10);
			VERIFY_COMPILE_TIME(res.denominator == 3);
			VERIFY_COMPILE_TIME(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		return success;
	}

	bool FractionTester::MultiplyTest()
	{
		{
			constexpr Fraction f1 = { 120, 45, true };
			constexpr Fraction f2 = { 4, 3, false };
			constexpr auto res = f1 * f2;

			VERIFY_COMPILE_TIME(res.numerator == 32);
			VERIFY_COMPILE_TIME(res.denominator == 9);
			VERIFY_COMPILE_TIME(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 120, 45 };
			constexpr Fraction f2 = 4;
			constexpr auto res = f1 * f2;

			VERIFY_COMPILE_TIME(res.numerator == 32);
			VERIFY_COMPILE_TIME(res.denominator == 3);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 12, 49 };
			constexpr Fraction f2 = { 49, 12 };
			constexpr auto res = f1 * f2;

			VERIFY_COMPILE_TIME(res.numerator == 1);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 9, 4 };
			constexpr Fraction f2 = { 16, 12 };
			constexpr auto res = f1 * f2;

			VERIFY_COMPILE_TIME(res.numerator == 3);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 9, 4 };
			constexpr Fraction f2 = 16;
			constexpr auto res = f1 * f2;

			VERIFY_COMPILE_TIME(res.numerator == 36);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) * (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		return success;
	}

	bool FractionTester::DivideTest()
	{
		{
			constexpr Fraction f1 = { 120, 45, true };
			constexpr Fraction f2 = { 4, 3, false };
			constexpr auto res = f1 / f2;

			VERIFY_COMPILE_TIME(res.numerator == 2);
			VERIFY_COMPILE_TIME(res.denominator == 1);
			VERIFY_COMPILE_TIME(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 120, 45 };
			constexpr Fraction f2 = 4;
			constexpr auto res = f1 / f2;

			VERIFY_COMPILE_TIME(res.numerator == 2);
			VERIFY_COMPILE_TIME(res.denominator == 3);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 12, 49 };
			constexpr Fraction f2 = { 49, 12, true };
			constexpr auto res = f1 / f2;

			VERIFY_COMPILE_TIME(res.numerator == 144);
			VERIFY_COMPILE_TIME(res.denominator == 2401);
			VERIFY_COMPILE_TIME(res.isNegative == true);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 9, 4 };
			constexpr Fraction f2 = { 16, 12 };
			constexpr auto res = f1 / f2;

			VERIFY_COMPILE_TIME(res.numerator == 27);
			VERIFY_COMPILE_TIME(res.denominator == 16);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		{
			constexpr Fraction f1 = { 9, 4 };
			constexpr Fraction f2 = 16;
			constexpr auto res = f1 / f2;

			VERIFY_COMPILE_TIME(res.numerator == 9);
			VERIFY_COMPILE_TIME(res.denominator == 64);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		return success;
	}

	bool FractionTester::NonSimplifiedTest()
	{
		{
			constexpr Fraction f1 = { 9, 4, false, false };
			constexpr Fraction f2 = 16;
			constexpr auto res = f1 / f2;

			VERIFY_COMPILE_TIME(res.numerator == 9);
			VERIFY_COMPILE_TIME(res.denominator == 64);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}
		
		{
			constexpr Fraction f1 = { 8, 4, false, false };
			constexpr Fraction f2 = 16;
			constexpr auto res = f1 / f2;

			VERIFY_COMPILE_TIME(res.numerator == 8);
			VERIFY_COMPILE_TIME(res.denominator == 64);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}
		
		{
			constexpr Fraction f1 = { 8, 4, false, false };
			constexpr Fraction f2 = { 18, 7, false };
			constexpr auto res = f1 * f2;

			VERIFY_COMPILE_TIME(res.numerator == 144);
			VERIFY_COMPILE_TIME(res.denominator == 28);
			VERIFY_COMPILE_TIME(res.isNegative == false);

			constexpr auto f2r = res.GetReal<float>();
			constexpr auto expected = (CAST(decltype(f2r), res.numerator) / res.denominator) / (res.isNegative ? -1 : 1);
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		return success;
	}


	bool FractionTester::RationalTest()
	{
		{
			constexpr Fraction f;
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 0);
			VERIFY_COMPILE_TIME(f.denominator == 1);

			constexpr auto f2r = f.GetReal<int>();
			VERIFY_COMPILE_TIME(f2r == 0);
		}

		{
			constexpr Fraction f = { 11, 2, false };
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 11);
			VERIFY_COMPILE_TIME(f.denominator == 2);

			constexpr auto f2r = f.GetReal<float>();
			VERIFY_COMPILE_TIME(f2r == 5.5f);
		}

		{
			constexpr Fraction f = { 1000, 250, true };
			VERIFY_COMPILE_TIME(f.isNegative == true);
			VERIFY_COMPILE_TIME(f.numerator == 4);
			VERIFY_COMPILE_TIME(f.denominator == 1);

			constexpr auto f2r = f.GetReal<float>();
			VERIFY_COMPILE_TIME(f2r == -4);
		}

		{
			constexpr Fraction f = { 1000, 250, true };
			VERIFY_COMPILE_TIME(f.isNegative == true);
			VERIFY_COMPILE_TIME(f.numerator == 4);
			VERIFY_COMPILE_TIME(f.denominator == 1);

			constexpr auto f2r = f.GetReal<unsigned>();
			VERIFY(f2r == 0);
		}

		{
			constexpr Fraction f = { 625, 1250, true };
			VERIFY_COMPILE_TIME(f.isNegative == true);
			VERIFY_COMPILE_TIME(f.numerator == 1);
			VERIFY_COMPILE_TIME(f.denominator == 2);

			constexpr auto f2r = f.GetReal<double>();
			VERIFY_COMPILE_TIME(f2r == -0.5);
		}

		return success;
	}

	bool FractionTester::IrrationalTest()
	{
		{
			constexpr Fraction f = { 333, 106, false };
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 333);
			VERIFY_COMPILE_TIME(f.denominator == 106);

			constexpr auto f2r = f.GetReal<int>();
			VERIFY_COMPILE_TIME(f2r == 3);
		}

		{
			constexpr Fraction f = { 333, 106, false };
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 333);
			VERIFY_COMPILE_TIME(f.denominator == 106);

			constexpr auto f2r = f.GetReal<double>();
			const auto f2rRounded = RoundTo4(f2r);
			const auto pi = M_PI;
			const auto piRounded = RoundTo4(pi);
			VERIFY(f2rRounded == piRounded);
		}

		{
			constexpr Fraction f = { 517656, 190435, false, false };
			VERIFY_COMPILE_TIME(f.isNegative == false);
			VERIFY_COMPILE_TIME(f.numerator == 517656);
			VERIFY_COMPILE_TIME(f.denominator == 190435);

			constexpr auto f2r = f.GetReal<double>();
			const auto f2rRounded = RoundTo4(f2r);
			const auto e = M_E;
			const auto eRounded = RoundTo4(e);
			VERIFY(f2rRounded == eRounded);
		}

		return success;
	}

	bool FractionTester::GetRealTest()
	{
		{
			constexpr auto f = Fraction{ 20, 4 };
			constexpr auto f2r = f.GetReal<float>();

			constexpr int8_t sign = f.GetSign();
			constexpr auto expected = (CAST(decltype(f2r), f.numerator) * sign) / f.denominator;
			VERIFY_COMPILE_TIME(f2r == expected);
		}
		
		{
			constexpr auto f = Fraction{ 5, 19 };
			constexpr auto f2r = f.GetReal<float>();

			constexpr int8_t sign = f.GetSign();
			constexpr auto expected = (CAST(decltype(f2r), f.numerator) * sign) / f.denominator;
			VERIFY_COMPILE_TIME(f2r == expected);
		}
		
		{
			constexpr auto f = Fraction{ 9, 81, true };
			constexpr auto f2r = f.GetReal<float>();

			constexpr int8_t sign = f.GetSign();
			constexpr auto expected = (CAST(decltype(f2r), f.numerator) * sign) / f.denominator;
			VERIFY_COMPILE_TIME(f2r == expected);
		}
		
		{
			constexpr auto f = Fraction{ 25, 81, true };
			constexpr auto f2r = f.GetReal<double>();

			constexpr int8_t sign = f.GetSign();
			constexpr auto expected = (CAST(decltype(f2r), f.numerator) * sign) / f.denominator;
			VERIFY_COMPILE_TIME(f2r == expected);
		}

		return success;
	}


	double FractionTester::RoundTo4(double x)
	{
		const auto isNegative = x < 0;
		if (isNegative)
			x = -x;

		const auto accuracy = 1e3;
		const auto dpShifts = 1e-4 * 5;

		const auto valuePlusDpsByAcc = (x + dpShifts) * accuracy;
		const auto accuracyInverse = 1.0 / accuracy;
		const auto significantFigures = int(valuePlusDpsByAcc);
		const auto roundedValue = significantFigures * accuracyInverse;
		return isNegative ? -roundedValue : roundedValue;
	}
}
#endif
