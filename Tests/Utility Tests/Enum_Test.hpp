#pragma once

#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class EnumTester : public Tester
	{
	public:
		EnumTester();
		~EnumTester() override;

	protected:
		void Test() override;

	private:
		bool FundamentalTest();
		bool ToStringTest();
		bool SizeOfTest();
		bool CompareTest();
		bool MaskCmpTest();
		bool ToEnumTest();
		bool ToUnderlyingTest();
	};
}
#endif
