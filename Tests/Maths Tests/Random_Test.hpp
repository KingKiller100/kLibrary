#pragma once

#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class RandomTester final : public Tester
	{
	public:
		RandomTester() ;
		~RandomTester() override;

	private:
		void Test() override;

		bool IntegerTest();
		bool FloatingPointTest();
	};
}
#endif
