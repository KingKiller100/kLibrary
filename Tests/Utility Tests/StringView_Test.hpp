#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringViewTester : public Tester
	{
	public:
		StringViewTester();
		~StringViewTester() override;

	private:
		void Test() override;
	};

}
#endif

