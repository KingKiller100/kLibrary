#include "pch.hpp"
#include "Algorithms_SpeedTest.hpp"

#include "../../../Source Files/Maths/kAlgorithms.hpp"

#include <cmath>


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	AlgorithmsSpeedTest::AlgorithmsSpeedTest()
		: PerformanceTestBase("Algorithms Speed Test")
	{}

	AlgorithmsSpeedTest::~AlgorithmsSpeedTest()
		= default;

	void AlgorithmsSpeedTest::Test()
	{
		PowerOfFractionsTest();
		PowerOfIntegersTest();
		SquareTest();
		SqrtTest();
		FloorTest();
		FloatingPointRemainderTest();
		SignTest();
		AbsTest();
		SineTest();
		CosineTest();
		TanTest();
	}

	constexpr auto maxIter = kmaths::Big_Int_Type(1e3);

	void AlgorithmsSpeedTest::PowerOfFractionsTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::PowerOf<double>", "std::pow<double>" };
		SetUpParticipants(participants);

		constexpr double set[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr auto size = kmaths::SizeOfCArray(set);
		constexpr int valueBeforeIteration = 1000;

		auto power = 0.0;
		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto number = set[idx];

			if (!(i % valueBeforeIteration))
				power += 1.35;

			{
				START_TEST(participants[0]);
				const auto value = kmaths::PowerOf(number, power);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::pow(number, power);
			}
		}
	}

	void AlgorithmsSpeedTest::PowerOfIntegersTest()
	{
		const std::vector<std::string_view> participants = {
			"kmaths::PowerOf<int>", "std::pow<int>",
			"kmaths::PowerOf<short>", "std::pow<short>"
		};
		SetUpParticipants(participants);

		constexpr int set[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr auto size = kmaths::SizeOfCArray(set);
		constexpr int valueBeforeIteration = 1000;

		kmaths::Big_Int_Type power = 0;
		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto number = set[idx];

			if (!(i % valueBeforeIteration))
				power++;

			{
				START_TEST(participants[0]);
				const auto value = kmaths::PowerOf(number, power);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::pow(number, power);
			}

			{
				START_TEST(participants[2]);
				const auto value = kmaths::PowerOf<short>(number, power);
			}

			{
				START_TEST(participants[3]);
				const auto value = std::pow<short>(number, power);
			}
		}
	}

	void AlgorithmsSpeedTest::SquareTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Square", "kmaths::PowerOfImpl", "std::pow" };
		SetUpParticipants(participants);

		constexpr kmaths::Big_Int_Type power = 2;

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto value = kmaths::Square(i);
			}

			{
				START_TEST(participants[1]);
				const auto value = kmaths::PowerOfImpl(i, power);
			}

			{
				START_TEST(participants[2]);
				const auto value = std::pow(i, power);
			}
		}
	}

	void AlgorithmsSpeedTest::SqrtTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Sqrt", "std::sqrt", "kmaths::Root" };
		SetUpParticipants(participants);

		constexpr double set[] = { 0, 1e0, 2e1, 3e2, 4e3, 5e4, 6e5, 7e6, 8e7, 9e8 };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto number = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Sqrt(number);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::sqrt(number);
			}

			{
				START_TEST(participants[2]);
				const auto value = kmaths::Root(number, 2);
			}
		}
	}

	void AlgorithmsSpeedTest::FloorTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Floor", "std::floor" };
		SetUpParticipants(participants);

		constexpr float set[] = { 0.2443f, 1.234543f, 2.7687f, 3.2342324f, 4.324543f, 5.5f, 6.354f, 7.234244554f, 8.4365f, 9.45434f };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto number = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Floor(number);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::floor(number);
			}
		}
	}
	void AlgorithmsSpeedTest::FloatingPointRemainderTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::FloatingPointRemainder", "std::fmod" };
		SetUpParticipants(participants);

		constexpr float set[] = { 0.2443f, 1.234543f, 2.7687f, 3.2342324f, 4.324543f, 5.5f, 6.354f, 7.234244554f, 8.4365f, 9.45434f };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto base = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::FloatingPointRemainder(CAST(float, i), base);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::fmod(CAST(float, i), base);
			}
		}
	}

	void AlgorithmsSpeedTest::SineTest()
	{
		constexpr auto pi_over_4 = kmaths::constants::PI_OVER_4;

		const std::vector<std::string_view> participants = { "kmaths::Sine", "std::sin" };
		SetUpParticipants(participants);

		for (auto i = 0; i < maxIter; ++i)
		{

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Sine(pi_over_4 * i);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::sin(pi_over_4 * i);
			}
		}
	}

	void AlgorithmsSpeedTest::CosineTest()
	{
		constexpr auto pi_over_4 = kmaths::constants::PI_OVER_4;

		const std::vector<std::string_view> participants = { "kmaths::Cosine", "std::cos" };
		SetUpParticipants(participants);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto value = kmaths::Cosine(pi_over_4 * i);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::cos(pi_over_4 * i);
			}
		}
	}

	void AlgorithmsSpeedTest::TanTest()
	{
		constexpr auto pi = kmaths::constants::PI;

		const std::vector<std::string_view> participants = { "kmaths::Tan", "std::tan" };
		SetUpParticipants(participants);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto value = kmaths::Tan(pi * i);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::tan(pi * i);
			}
		}
	}

	void AlgorithmsSpeedTest::SignTest()
	{
		const std::vector<std::string_view> participants = { "kmaths::Sign", "std::copysign" };
		SetUpParticipants(participants);

		constexpr float set[] = { -600, -1, -0.25, 0, 0.25, 1, 600 };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto num = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Sign(num);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::copysign(num, num);
			}
		}
	}

	void AlgorithmsSpeedTest::AbsTest()
	{
		const std::vector<std::string_view> participants = {
			"kmaths::Abs<long double>", "std::fabsl",
			"kmaths::Abs<double>", "std::fabs",
			"kmaths::Abs<float>", "std::fabsf",
			"kmaths::Abs<kmaths::Big_Int_Type>", "std::fabsf<kmaths::Big_Int_Type>",
		};
		SetUpParticipants(participants);

		constexpr long double set[] = { -103.856, -71, -1, 0, 1, 349, 1035.456 };
		constexpr auto size = kmaths::SizeOfCArray(set);

		for (auto i = 0; i < maxIter; ++i)
		{
			const auto idx = kmaths::Modulus<int>(i, size);
			const auto num = set[idx];

			{
				START_TEST(participants[0]);
				const auto value = kmaths::Abs<long double>(num);
			}

			{
				START_TEST(participants[1]);
				const auto value = std::fabsl(num);
			}

			{
				START_TEST(participants[2]);
				const auto value = kmaths::Abs<double>(num);
			}

			{
				START_TEST(participants[3]);
				const auto value = std::fabs(num);
			}

			{
				START_TEST(participants[4]);
				const auto value = kmaths::Abs<float>(CAST(float, num));
			}

			{
				START_TEST(participants[5]);
				const auto value = std::fabsf(CAST(float, num));
			}

			{
				START_TEST(participants[6]);
				const auto value = kmaths::Abs<kmaths::Big_Int_Type>(CAST(kmaths::Big_Int_Type, num));
			}

			{
				START_TEST(participants[7]);
				const auto value = std::fabs<kmaths::Big_Int_Type>(CAST(kmaths::Big_Int_Type, num));
			}
		}
	}

}
#endif
