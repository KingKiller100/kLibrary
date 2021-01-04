#pragma once

#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringManipulationTester final : public TesterBase
	{
	public:
		StringManipulationTester();
		~StringManipulationTester();

	private:
		void Test() override;

		bool StrToTest();
		bool CountTest();
		bool ReplaceTest();
		bool RemoveTest();
		bool SplitTest();
		bool ToLower();
		bool ToUpper();
		bool ToWriterAndToReaderTest();
		bool IsWhiteSpaceOrNullTest();
		bool FindTest();
		bool FindFirstOfTest();
		bool FindFirstNotOfTest();
		bool FindLastOfTest();
		bool FindLastNotOfTest();
	};

}
#endif