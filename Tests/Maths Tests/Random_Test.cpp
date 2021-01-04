#include "pch.hpp"
#include "Random_Test.hpp"

#ifdef TESTING_ENABLED

#include "../../Source//Maths/Length_Type.hpp"
#include "../../Source//Maths/Random.hpp"

#include <thread>

namespace kTest::maths
{
	RandomTester::RandomTester()
		: TesterBase("Random Number Tester")
	{}

	RandomTester::~RandomTester()
		= default;

	using namespace kmaths;
	using namespace kmaths::kRng;
	void RandomTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(IntegerBCryptTest());
		VERIFY_MULTI(FloatingBCryptPointTest());

		VERIFY_MULTI(IntegerTimeTest());
		VERIFY_MULTI(FloatingTimePointTest());
		VERIFY_MULTI_END();
	}

	bool RandomTester::IntegerBCryptTest()
	{
		using namespace std::chrono_literals;

		{
			constexpr BigInt_t lb = 0;
			constexpr BigInt_t ub = 100;

			auto rng = RngU32(GenericSeedingSource::BCRYPT);

			const auto a = rng(lb, ub);
			const auto b = rng(lb, ub);

			VERIFY(a != b);
			VERIFY(lb <= a&& a <= ub);
			VERIFY(lb <= b&& b <= ub);
		}

		{
			constexpr BigInt_t lb = 0;
			constexpr BigInt_t ub = 100;

			auto rng = RngU64(GenericSeedingSource::BCRYPT);

			const auto a = rng(lb, ub);
			const auto b = rng(lb, ub);

			VERIFY(a != b);
			VERIFY(lb <= a && a <= ub);
			VERIFY(lb <= b && b <= ub);
		}

		return success;
	}

	bool RandomTester::IntegerTimeTest()
	{
		using namespace std::chrono_literals;
		{
			constexpr BigInt_t lb = 0;
			constexpr BigInt_t ub = 100;

			auto rng = RngU32(GenericSeedingSource::TIME);

			const auto a = rng.Generate(lb, ub);
			//std::this_thread::sleep_for(1s);
			const auto b = rng.Generate(lb, ub);


			VERIFY(a != b);
			VERIFY(lb <= a&& a <= ub);
			VERIFY(lb <= b&& b <= ub);
		}

		{
			constexpr BigInt_t lb = 0;
			constexpr BigInt_t ub = 100;

			auto rng = RngU64(GenericSeedingSource::TIME);

			const auto a = rng.Generate(lb, ub);
			//std::this_thread::sleep_for(1s);
			const auto b = rng.Generate(lb, ub);


			VERIFY(a != b);
			VERIFY(lb <= a&& a <= ub);
			VERIFY(lb <= b&& b <= ub);
		}

		return success;

	}

	bool RandomTester::FloatingBCryptPointTest()
	{
		using namespace std::chrono_literals;

		{
			constexpr float lb = 0.25f;
			constexpr float ub = 0.5f;

			auto rng = RngU32(GenericSeedingSource::BCRYPT);
			const auto a = rng(lb, ub);
			const auto b = rng(lb, ub);

			VERIFY(a != b);
			VERIFY(lb <= a&& a <= ub);
			VERIFY(lb <= b&& b <= ub);
		}

		{
			constexpr auto lb = 18.25;
			constexpr auto ub = 50.75;

			auto rng = RngU64(GenericSeedingSource::BCRYPT);
			const auto a = rng(lb, ub);
			const auto b = rng(lb, ub);

			VERIFY(a != b);
			VERIFY(lb <= a&& a <= ub);
			VERIFY(lb <= b&& b <= ub);
		}

		return success;
	}


	bool RandomTester::FloatingTimePointTest()
	{
		using namespace std::chrono_literals;

		{
			constexpr float lb = 0.25f;
			constexpr float ub = 0.5f;

			auto rng = RngU32(GenericSeedingSource::TIME);
			const auto a = rng(lb, ub);
			const auto b = rng(lb, ub);

			VERIFY(a != b);
			VERIFY(lb <= a&& a <= ub);
			VERIFY(lb <= b&& b <= ub);
		}

		{
			constexpr auto lb = 18.25;
			constexpr auto ub = 50.75;

			auto rng = RngU64(GenericSeedingSource::TIME);
			const auto a = rng(lb, ub);
			const auto b = rng(lb, ub);

			VERIFY(a != b);
			VERIFY(lb <= a&& a <= ub);
			VERIFY(lb <= b&& b <= ub);
		}

		return success;
	}
}
#endif


