#pragma once

#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class EnumTester final : public Tester
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
		bool PrettyTypeTest();
		bool PrettyValueTest();
		bool FromStringTest();
	};
}
#endif
