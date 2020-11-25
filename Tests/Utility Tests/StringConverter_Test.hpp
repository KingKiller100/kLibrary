#pragma once

#include "../../Source Files/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringConverterTester : public TesterBase
	{
	public:
		StringConverterTester();
		~StringConverterTester() override;

	protected:
		void Test() override;
	};
}
#endif
