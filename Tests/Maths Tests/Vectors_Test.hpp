#pragma once
#include "../../Source Files/Testing/Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class VectorsTester final : public Tester
	{
	public:
		VectorsTester();
		~VectorsTester() override;

	protected:
		void Test() override;
		
	private:
		bool VectorTest();
		bool Vector2Test();
		bool Vector3Test();
		bool Vector4Test();
	};
}
#endif
