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
		void Prepare() noexcept override;

	private:
		void DynamicMatrixTest();
		void AddTest();
		void SubtractTest();
		void DivideTest();
		void MultiplyTest();
		void ConstexprTest();
		void InitializerListTest();
	};
}
#endif