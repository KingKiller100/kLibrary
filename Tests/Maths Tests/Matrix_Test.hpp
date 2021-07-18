#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class MatricesTester final : public TesterBase
	{
	public:
		MatricesTester();
		~MatricesTester() override;

	protected:
		void Prepare() override;

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