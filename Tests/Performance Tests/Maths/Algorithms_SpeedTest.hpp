#pragma once

#include "../PerformanceTestBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	class AlgorithmsSpeedTest : public PerformanceTestBase
	{
	public:
		AlgorithmsSpeedTest();
		~AlgorithmsSpeedTest();

	private:
		void Test() override;

		void PowerOfFractionsTest();
		void PowerOfIntegersTest();
		void SquareTest();
		void SignTest();
		void AbsTest();
		void SqrtTest();
		void FloorTest();
		void FloatingPointRemainderTest();
		void SineTest();
		void CosineTest();
		void TanTest();
	};
}
#endif
