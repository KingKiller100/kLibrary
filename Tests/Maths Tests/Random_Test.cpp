#include "pch.hpp"
#include "Random_Test.hpp"

#ifdef TESTING_ENABLED

#include "../../Source Files//Maths/Length_Type.hpp"
#include "../../Source Files//Maths/Random.hpp"

#include <thread>

namespace kTest::maths
{
	RandomTester::RandomTester()
		: Tester("Random Number Tester")
	{}

	RandomTester::~RandomTester()
		= default;

	using namespace kmaths;
	void RandomTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(IntegerTest());
		VERIFY_MULTI(FloatingPointTest());
		VERIFY_MULTI_END();
	}

	bool RandomTester::IntegerTest()
	{
		using namespace std::chrono_literals;
		
		constexpr Big_Int_Type lb = 0;
		constexpr Big_Int_Type ub = 100;

		const auto a = RNG(lb, ub);
		std::this_thread::sleep_for(1s);
		const auto b = RNG(lb, ub);

		VERIFY(a != b);
		VERIFY(lb < a && a < ub);
		VERIFY(lb < b && b < ub);

		return success;
	}

	bool RandomTester::FloatingPointTest()
	{
		using namespace std::chrono_literals;
		constexpr float lb = 0.25f;
		constexpr float ub = 0.5f;

		const auto a = RNG(lb, ub);
		std::this_thread::sleep_for(1s);
		const auto b = RNG(lb, ub);

		VERIFY(a != b);
		VERIFY(lb < a && a < ub);
		VERIFY(lb < b && b < ub);

		return success;
	}
}
#endif


