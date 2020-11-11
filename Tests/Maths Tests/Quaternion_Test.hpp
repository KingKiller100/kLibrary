#pragma once
#include "../../Source Files/Testing/Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class QuaternionsTester : public Tester
	{
	public:
		QuaternionsTester();
		~QuaternionsTester() override;

	private:
		void Test() override;
	};
}
#endif