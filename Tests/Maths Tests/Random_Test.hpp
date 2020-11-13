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

	protected:
		void Test() override;

	private:
		bool IntegerTest();
		bool FloatingPointTest();
	};
}
#endif
