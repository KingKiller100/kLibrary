#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class FormatToStringTester : public Tester
	{
	public:
		FormatToStringTester();
		~FormatToStringTester();

	private:
		void Test() override;

		bool IdentityTest();
		bool SprintfWrapperTest();
		bool StringifyHexTest();
		bool StringifyBinaryTest();
		bool CustomTypeWithToStringTest();
		bool CustomTypeWithoutToStringTest();
		bool FormatToStringTest();
		bool DirectToStringTest();
	};
}
#endif