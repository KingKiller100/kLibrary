#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringViewTester : public TesterBase
	{
	public:
		StringViewTester();
		~StringViewTester() override;

	private:
		void Prepare() override;
	};

}
#endif

