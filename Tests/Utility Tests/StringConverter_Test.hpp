#pragma once

#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringConverterTester : public TesterBase
	{
	public:
		StringConverterTester();
		~StringConverterTester() override;

	protected:
		void Prepare() override;
	};
}
#endif
