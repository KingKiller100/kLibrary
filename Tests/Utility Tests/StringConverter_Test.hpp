#pragma once

#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringConverterTester : public Tester
	{
	public:
		StringConverterTester();
		~StringConverterTester() override;

	protected:
		void Test() override;
	};
}
#endif
