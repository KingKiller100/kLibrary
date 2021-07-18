#pragma once
#include "../../Source/Testing/TesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class QuaternionsTester : public TesterBase
	{
	public:
		QuaternionsTester();
		~QuaternionsTester() override;

	protected:
		void Prepare() override;
	};
}
#endif