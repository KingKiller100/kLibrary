#pragma once
#include "../../Source/Testing/TesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class VectorsTester final : public TesterBase
	{
	public:
		VectorsTester();
		~VectorsTester() override;

	protected:
		void Prepare() noexcept override;
		
	private:
		bool VectorTest();
		bool Vector2Test();
		bool Vector3Test();
		bool Vector4Test();
	};
}
#endif
