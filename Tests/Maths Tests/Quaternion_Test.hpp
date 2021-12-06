#pragma once
#include "../IncludeTesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class QuaternionsTester : public TesterBase
	{
	public:
		QuaternionsTester();
		~QuaternionsTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void QuaToMat();
	};
}
#endif