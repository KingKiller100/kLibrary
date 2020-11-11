#pragma once
#include "../../Source Files/Testing/Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class AlgorithmsTester final : public Tester
	{
	public:
		AlgorithmsTester();
		~AlgorithmsTester() override;

	private:
		void Test() override;

		bool CountTest();
		bool SineTest();
		bool CosineTest();
		bool TanTest();
		bool ConstantsTest();
		bool ConversionTest();
		bool CountDigitsTest();
		bool SignTest();
		bool AbsTest();
		bool IsNegativeTest();
		bool MinMaxTest();
		bool FloorTest();
		bool RealToFractionTest();
		bool BinarySearchTest();
		bool BinarySearchClosestTest();
		bool RoundingTest();
		bool PowerOfTest();
		bool SwapTest();
		bool GammaTest();
		bool FactorialTest();
		bool ClampTest();
		bool AbsClampTest();
		bool ToDegreesTest();
		bool ToRadiansTest();
		bool SquareRootTest();
		bool InverseSquareRootTest();
		bool RootTest();
		bool Log10Test();
		bool Log2Test();
		bool PowerOfFractionTest();
		bool FloatingPointRemainderTest();
		bool ModulusTest();
	};
}
#endif