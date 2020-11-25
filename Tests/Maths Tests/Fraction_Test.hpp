#pragma once
#include "../../Source Files/Testing/TesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class FractionTester : public TesterBase
	{
	public:
		FractionTester();
		~FractionTester() override;

	protected:
		void Test() override;

	private:
		double RoundTo4(double x);
		
		bool AddTest();
		bool SubtractTest();
		bool MultiplyTest();
		bool DivideTest();
		bool NonSimplifiedTest();
		bool RationalTest();
		bool IrrationalTest();
		bool GetRealTest();

	};
}
#endif