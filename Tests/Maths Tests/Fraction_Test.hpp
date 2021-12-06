#pragma once
#include "../IncludeTesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class FractionTester : public TesterBase
	{
	public:
		FractionTester();
		~FractionTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		double RoundTo4(double x);
		
		void AddTest();
		void SubtractTest();
		void MultiplyTest();
		void DivideTest();
		void NonSimplifiedTest();
		void RationalTest();
		void IrrationalTest();
		void GetRealTest();

	};
}
#endif