#pragma once
#include "../IncludeTesterBase.hpp"


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
		void VectorTest();
		void Vector2Test();
		void Vector3Test();
		void Vector4Test();
	};
}
#endif
