#pragma once

#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class RandomTester final : public TesterBase
	{
	public:
		RandomTester() ;
		~RandomTester() override;

	protected:
		void Test() override;

	private:
		bool IntegerBCryptTest();
		bool IntegerTimeTest();
		bool FloatingBCryptPointTest();
		bool FloatingTimePointTest();
	};
}
#endif
