#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class MatricesTester final : public Tester
	{
	public:
		MatricesTester();
		~MatricesTester() override;

	protected:
		void Test() override;

	private:
		bool DynamicMatrixTest();
		bool AddTest();
		bool SubtractTest();
		bool DivideTest();
		bool MultiplyTest();
		bool ConstexprTest();
		bool InitializerListTest();
	};
}
#endif