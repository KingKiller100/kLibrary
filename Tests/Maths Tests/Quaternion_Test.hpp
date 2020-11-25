#pragma once
#include "../../Source Files/Testing/TesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class QuaternionsTester : public TesterBase
	{
	public:
		QuaternionsTester();
		~QuaternionsTester() override;

	protected:
		void Test() override;
	};
}
#endif