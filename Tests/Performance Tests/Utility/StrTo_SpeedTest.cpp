#include "pch.hpp"
#include "StrTo_SpeedTest.hpp"

#include "../../../Source/Utility/String/kStringTricks.hpp"
#include <string>

namespace kTest::performance::util
{
	using namespace klib::kString;
	
	constexpr auto maxIter = static_cast<size_t>(1e4);

	StrToSpeedTest::StrToSpeedTest() noexcept
		: PerformanceTestBase("StrTo Speed Test")
	{}

	StrToSpeedTest::~StrToSpeedTest() noexcept
		= default;

	void StrToSpeedTest::Test()
	{
		StrToInt();
		StrToFloat();
		StrToLongLong();
		StrToDouble();
	}

	void StrToSpeedTest::StrToInt()
	{
		const std::vector<std::string_view> participants = { "klib::kString::StrTo<int>", "std::stoi" };
		SetUpParticipants(participants);

		const char str[] = "100";
		
		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				(void)StrTo<int, std::string>(str);
			}

			{
				START_TEST(participants[1]);
				(void)std::stoi(str);
			}
		}
	}

	void StrToSpeedTest::StrToLongLong()
	{
		const std::vector<std::string_view> participants = { "klib::kString::StrTo<long long>", "std::stoll" };
		SetUpParticipants(participants);

		const std::string str = "100640";

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				(void)StrTo<long long>(str);
			}

			{
				START_TEST(participants[1]);
				(void)std::stoll(str);
			}
		}
	}

	void StrToSpeedTest::StrToFloat()
	{
		const std::vector<std::string_view> participants = { "klib::kString::StrTo<float>", "std::stof" };
		SetUpParticipants(participants);

		const std::string str = "100.5";

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				(void)StrTo<float>(str);
			}

			{
				START_TEST(participants[1]);
				(void)std::stof(str);
			}
		}
	}

	void StrToSpeedTest::StrToDouble()
	{
		const std::vector<std::string_view> participants = { "klib::kString::StrTo<double>", "std::stod" };
		SetUpParticipants(participants);

		const std::string str = "100.0625";

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				(void)StrTo<double>(str);
			}

			{
				START_TEST(participants[1]);
				(void)std::stod(str);
			}
		}
	}
}
