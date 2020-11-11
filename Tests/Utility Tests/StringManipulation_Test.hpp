#pragma once

#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringManipulationTester final : public Tester
	{
	public:
		StringManipulationTester();
		~StringManipulationTester();

	private:
		void Test() override;

		bool ReplaceTest();
		bool SplitTest();
		bool ToLower();
		bool ToUpper();
		bool ToWriterAndToReaderTest();
	};

}
#endif