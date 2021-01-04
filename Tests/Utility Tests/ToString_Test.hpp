#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class FormatToStringTester : public TesterBase
	{
	public:
		FormatToStringTester();
		~FormatToStringTester();

	private:
		void Test() override;

		bool IdentityTest();
		bool SprintfWrapperTest();
		bool StringifyFloatingPointTest();
		bool StringifyHexTest();
		bool StringifyBinaryTest();
		bool CustomTypeWithToStringTest();
		bool CustomTypeWithoutToStringTest();
		bool FormatToStringTest();
		bool DirectToStringTest();
	};
}
#endif