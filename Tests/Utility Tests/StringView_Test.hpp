#pragma once
#include "../../Source Files/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringViewTester : public TesterBase
	{
	public:
		StringViewTester();
		~StringViewTester() override;

	private:
		void Test() override;
	};

}
#endif

