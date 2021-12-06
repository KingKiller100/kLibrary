#pragma once
#include "../IncludeTesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class AlgorithmsTester final : public TesterBase
	{
	public:
		AlgorithmsTester();
		~AlgorithmsTester();

	protected:
		void Prepare() noexcept override;

	private:
		void CountTest();
		void SineTest();
		void CosineTest();
		void TanTest();
		void ConstantsTest();
		void ConversionTest();
		void CountDigitsTest();
		void SignTest();
		void AbsTest();
		void IsNegativeTest();
		void MinMaxTest();
		void FloorTest();
		void RealToFractionTest();
		void BinarySearchTest();
		void BinarySearchClosestTest();
		void RoundingTest();
		void PowerOfTest();
		void SwapTest();
		void GammaTest();
		void FactorialTest();
		void ClampTest();
		void AbsClampTest();
		void ToDegreesTest();
		void ToRadiansTest();
		void SquareRootTest();
		void InverseSquareRootTest();
		void RootTest();
		void Log10Test();
		void Log2Test();
		void PowerOfFractionTest();
		void FloatingPointRemainderTest();
		void ModulusTest();
		void IsInfTest();
		void IsNaNTest();
	};
}
#endif