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
		void Prepare() noexcept override;

		void StrToTest();
		void CountTest();
		void ReplaceTest();
		void RemoveTest();
		void SplitTest();
		void ToLower();
		void ToUpper();
		void ToWriterAndToReaderTest();
		void IsWhiteSpaceOrNullTest();
		void FindTest();
		void FindFirstOfTest();
		void FindFirstNotOfTest();
		void FindLastOfTest();
		void FindLastNotOfTest();
	};

}
#endif